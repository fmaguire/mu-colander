#ifndef U_COLANDER_SRC_UTILS_H_
#define U_COLANDER_SRC_UTILS_H_
#include "opencv2/videoio.hpp"
#include "opencv2/core.hpp"

void create_windows();
void add_frame_number(cv::VideoCapture vidcap, cv::Mat vidframe);

#endif //U_COLANDER_SRC_UTILS_H_
