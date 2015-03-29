#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <sstream>

void create_windows() {
    //create GUI windows
    cv::namedWindow("frame");
    cv::namedWindow("fg_mask");
    //cv::namedWindow("fg_img");
    cv::namedWindow("roi");
}

void add_frame_number(cv::VideoCapture vidcap, cv::Mat vidframe) {
    //get the frame number and write it on the current frame
    std::stringstream ss;
    cv::rectangle(vidframe, cv::Point(10, 2), cv::Point(100,20),
              cv::Scalar(255,255,255), -1);
    ss << vidcap.get(cv::CAP_PROP_POS_FRAMES);
    std::string frameNumberString = ss.str();
    cv::putText(vidframe, frameNumberString.c_str(), cv::Point(15, 15),
            cv::FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
}
