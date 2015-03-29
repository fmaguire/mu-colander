#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>

#include <stdio.h>

#include <iostream>

#include "utils.hpp"

void processVideo(char* videoFilename) {

    cv::Mat frame; 
    cv::Mat fg_mask; 
    cv::Mat fg_img;
    //cv::Mat bg_img;


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
        cv::GaussianBlur(fg_mask, fg_mask, cv::Size(5, 5), 4, 4);

        fg_img = cv::Scalar::all(0);
        frame.copyTo(fg_img, fg_mask);
        image_roi = fg_img(roi);
        
        add_frame_number(capture, frame);

        //show the current frame and the fg masks
        cv::imshow("frame", frame);
        //cv::imshow("fg_img", fg_img);
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
        keyboard = cv::waitKey( 30);
        if ( (char)keyboard == '.' ) {
            keyboard = cv::waitKey(0);
        }
    }
    //delete capture object
    capture.release();
}
