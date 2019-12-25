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

class KScope {

private:

  String WINDOW_NAME = "Kaleidoscope";
  
  Mat base_img, show_img;
  int width, height;

  int n_frames , f_start_ind , n_interval;
  


  system_clock::time_point t_start = system_clock::now(), t_end;

  void _reset_frame_counter(void);
  int _show_frame(Mat image, int delay=0);
  
public:
  KScope(char * img_name);
  ~KScope();
  void run();
  
  float get_frame_rate();
  

};

#endif //KSCOPE_HPP_D
