//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/video/video.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>

/** Notes
 * 
 *
 *
 */

/** Function Headers */
void processVideo(char* videoFilename);
void create_windows();
void add_frame_number(cv::VideoCapture vidcap, cv::Mat vidframe);

/** Main */
int main(int argc, char* argv[])
{

    //check for the input parameter correctness
    if(argc != 2 || strcmp(argv[1], "-h") == 0) {
        std::cerr << "Usage: " << argv[0] << " video.avi" << std::endl;
        std::exit(1);
    }
    
    create_windows();
    processVideo(argv[1]);
    
    //destroy GUI windows
    cv::destroyAllWindows();
    return 0;
}

void create_windows() {
    //create GUI windows
    cv::namedWindow("frame");
    cv::namedWindow("fg_mask");
    cv::namedWindow("fg_img");
    cv::namedWindow("bg_img");
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

void processVideo(char* videoFilename) {

    cv::Mat frame; 
    cv::Mat fg_mask; 
    cv::Mat fg_img;
    cv::Mat bg_img;


    int out_index;
    cv::Ptr<cv::BackgroundSubtractor> pKNN = cv::createBackgroundSubtractorKNN();
    
    int keyboard; //input from keyboard
    

    //create the capture object
    cv::VideoCapture capture(videoFilename);
    if(!capture.isOpened()){
        //error in opening the video input
        std::cerr << "Unable to open video file: " << videoFilename << std::endl;
        std::exit(1);
    }

    //read input data. ESC or 'q' for quitting
    while( (char)keyboard != 'q' && (char)keyboard != 27 ){
        

        //read the current frame
        if(!capture.read(frame)) {
            std::cerr << "Unable to read next frame." << std::endl;
            std::cerr << "Video Finished: Exiting." << std::endl;
            std::exit(0);
        }
        
        cv::Size frame_dim = frame.size();
        cv::Rect roi(frame_dim.width / 2, 0, 50, frame_dim.height);
        cv::Mat image_roi;
        
        // place region of interest

        //update the background model
        pKNN->apply(frame, fg_mask);
        
        

        //apply cleaning kernel
        cv::threshold(fg_mask, fg_mask, 10, 255, 0);
        cv::GaussianBlur(fg_mask, fg_mask, cv::Size(5, 5), 3,3);
        
        fg_img = cv::Scalar::all(0);
        frame.copyTo(fg_img, fg_mask);
        image_roi = fg_img(roi);
        
        add_frame_number(capture, frame);

        //show the current frame and the fg masks
        cv::imshow("frame", frame);
        cv::imshow("fg_img", fg_img);
        cv::imshow("fg_mask", fg_mask);

        cv::imshow("roi", image_roi);

        int presence = cv::sum(cv::sum(image_roi))[0];
        if (presence > 0){
            std::ostringstream fn;
            fn << "out/image_" << out_index << ".png";
            cv::imwrite(fn.str(), image_roi);
            out_index++;
        }

        ////get the input from the keyboard
        keyboard = cv::waitKey( 1 );
        if ( (char)keyboard == '.' ) {
            keyboard = cv::waitKey(0);
        }
    }
    //delete capture object
    capture.release();
}
