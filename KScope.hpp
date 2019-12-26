#ifndef KSCOPE_HPP_D
#define KSCOPE_HPP_D
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <string>
using namespace cv;
using namespace std;
using namespace std::chrono;

void _mouse_callback(int event, int x, int y, int flags, void* userdata);


typedef struct ray_str{
  Mat pt, dir;
} ray;


class Mirror{
public:
  ray norm;
  double height;
  Mirror(double z_dist, ray normal);
  Mirror(double z_dist, Mat p0, Mat p1);
  ~Mirror();
  Mat intersect_rays(vector<ray> rays);
};


class KScope {

private:

  String IMG_WINDOW_NAME = "Target Image";
  String K_WINDOW_NAME = "Kaleidoscope";
  std::vector<Mirror*> * mirrors;
  
  Mat base_img, show_img;
  int n_frames , f_start_ind , n_interval;
  system_clock::time_point t_start = system_clock::now(), t_end;
  void _reset_frame_counter(void);
  int _show_frame(Mat image, int delay=0);
  double z_dist;
public:
  void print_mirrors();
  void iso_triangle(double height, double base);

  KScope(char * img_name);
  ~KScope();
  void run();
  
  float get_frame_rate();
  

};

#endif //KSCOPE_HPP_D
