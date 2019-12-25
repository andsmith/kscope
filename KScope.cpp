#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <chrono>
#include "KScope.hpp"
using namespace std;
using namespace std::chrono;


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
  
  // cv2 window stuff
  namedWindow(WINDOW_NAME, WINDOW_NORMAL );
  //setWindowProperty(WINDOW_NAME, WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);    
  setMouseCallback( WINDOW_NAME, _mouse_callback, this);
}


int KScope::_show_frame(Mat image, int delay){
  imshow(WINDOW_NAME, image);
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
    scope.run();
    
    return 0;
}

