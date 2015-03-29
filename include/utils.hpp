#ifndef U_COLANDER_SRC_UTILS_H_
#define U_COLANDER_SRC_UTILS_H_
#include "opencv2/videoio.hpp"
#include "opencv2/core.hpp"

void create_windows();
void add_frame_number(cv::VideoCapture vidcap, cv::Mat vidframe);
cv::Mat next_frame(cv::VideoCapture vidcap);
cv::VideoCapture parse_video(char* filename);

#endif //U_COLANDER_SRC_UTILS_H_
