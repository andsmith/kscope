#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <chrono>
#include "KScope.hpp"
using namespace std;
using namespace std::chrono;

/*
class Mirror{
public:
  Mat normal, center;
  Mirror(Mat normal, Mat center);
  ~Mirror();
  Mat intersect_rays(Mat rays);
};*/


Mirror::~Mirror(){
}

Mirror::Mirror(double z_dist, ray normal){
  norm=normal;
  height = z_dist;
}

Mirror::Mirror(double z_dist, Mat p0, Mat p1){
  Mat p0_3d = Mat::zeros(3,1, CV_32F);
  Mat p1_3d = Mat::zeros(3,1, CV_32F);
  height = z_dist;
  /*
  p0_3d.at<double>(0,0) = 1.0;//p0.at<double>(0,0);
  p0_3d.at<double>(1,0) = 2.0;//p0.at<double>(1,0);
  p0_3d.at<double>(2,0) = 3.3;//z_dist/2.0;
  p1_3d.at<double>(0,0) = 4.4;//p1.at<double>(0,0);
  p1_3d.at<double>(1,0) = 5.5;//p1.at<double>(1,0);
  p1_3d.at<double>(2,0) = 6.6;//z_dist/2.0;
  */
  cout<<"p0: "<< p0*1000 << endl<<std::flush;
  cout<<"p1: "<< p1*1000 << endl<<std::flush;
  cout<<"p0_3d: "<< p0_3d*1000 << endl<<std::flush;
  cout<<"p1_3d: "<< p1_3d*1000 << endl<<std::flush;
  
  
  Mat center = (p0_3d + p1_3d) / 2.0;
  Mat parallel = (p1_3d - p0_3d);
  Mat origin = Mat::zeros(3,1, CV_32F);
  Mat to_center = (origin - center);
  Mat perp = to_center - to_center * to_center.dot(parallel);
  cout<<"center: " << center<<endl<<std::flush;
  cout<<"parallel:  " << parallel<<endl << std::flush;
  cout<<"origin: " << origin<<endl<<std::flush;
  cout<<"to_center:  " << to_center<<endl << std::flush;
  cout<<"perp: " << perp<<endl<<std::flush;


  cout<<"A" << perp<<endl<<std::flush;
  cout<<"B" << sum(perp.mul(perp)) << std::flush;
  cout<<"C" << endl<<std::flush;
  cout<<"D" <<sqrt(4.0) << endl<<std::flush;
  cout<<"E" << endl<<std::flush;
    
  perp = perp / sqrt((double)(sum((perp*perp))[0]));
  cout<<"B"<<endl;
  norm.pt = center;
  norm.dir = perp;
  float check = perp.dot(parallel);

  cout << "Checking perpendicularity: " << check << endl;
  
}


float KScope::get_frame_rate(){
  int n_f = n_frames - f_start_ind;
  t_end =  system_clock::now();
  chrono::duration<double> t_elapsed = t_end - t_start;
  return ((float)n_f) /  t_elapsed.count();
}

void KScope::_reset_frame_counter(){
  t_start = t_end;
  f_start_ind = n_frames;
}

void _mouse_callback(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_MBUTTONDOWN )
     {
          cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if ( event == EVENT_MOUSEMOVE )
     {
          cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

     }
}


KScope::KScope(char* img_name){
  base_img = imread(img_name,1);
  cout<<"Loaded image:  "<<base_img.cols<<" x "<<base_img.rows<<endl;
  n_interval=50;
  n_frames=0;
  z_dist = 20.0;
  // cv2 window stuff
  namedWindow(IMG_WINDOW_NAME, WINDOW_NORMAL );
  //setWindowProperty(WINDOW_NAME, WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);    
  setMouseCallback(IMG_WINDOW_NAME, _mouse_callback, this);
}

void KScope::print_mirrors(){
  for (int i=0;i<mirrors->size(); i++){
    cout<<i<<endl;
  }
}

void KScope::iso_triangle(double height, double base){
  vector<Mirror*> *mrs = new vector<Mirror*>;

  Mat p0 = Mat(2,1, CV_64F);
  Mat p1 = Mat(2,1, CV_64F);
  Mat p2 = Mat(2,1, CV_64F);
  p0.at<double>(0,0) = 0.0;
  p0.at<double>(1,0) = height;
  p1.at<double>(0,0) = -base/2.0;
  p1.at<double>(1,0) = 0.0;
  p2.at<double>(0,0) = base / 2.0;
  p2.at<double>(1,0) = 0.0;
  //cout<<"iso p0: "<< p0*1000 << endl<<std::flush;
  //cout<<"iso p1: "<< p1*1000 << endl<<std::flush;
  //cout<<"iso p2: "<< p2*1000 << endl<<std::flush;
  Mirror *m1 = new Mirror(z_dist, p0, p1);
  Mirror *m2 = new Mirror(z_dist, p1, p2);
  Mirror *m3 = new Mirror(z_dist, p2, p0);
  mrs->push_back(m1);
  mrs->push_back(m2);
  mrs->push_back(m3);
  mirrors= mrs;
}


int KScope::_show_frame(Mat image, int delay){
  imshow(IMG_WINDOW_NAME, image);
  return waitKey(delay);
}

KScope::~KScope(){
}


void KScope::run(){
    int keystroke=0;
  
  while (1){
    
    n_frames++;
    
    keystroke = _show_frame(base_img, 1);
    
    if (n_frames%n_interval==0) {
      cout<<"Frame rate:  "<<get_frame_rate()<<"  "<<n_frames<<endl;
      _reset_frame_counter();
    }
    
    if (keystroke == 27 || keystroke == 113){
      break;
    } else {
      if (keystroke != 0 && keystroke != -1)
	cout<<"Key:  "<<keystroke<<endl;
    }

  }
}

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: KScope  <image_file_path>\n");
        return -1;
    }


    KScope scope(argv[1]);
    scope.iso_triangle(4.0, 2.0);
    scope.print_mirrors();
    scope.run();
    
    return 0;
}

