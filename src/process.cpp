#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>

#include <stdio.h>

#include <iostream>

#include "utils.hpp"

cv::Mat filter(cv::Ptr<cv::BackgroundSubtractor> subtractor,
               cv::Mat current_frame, 
               cv::Mat current_mask){

    //update the background model
    subtractor->apply(current_frame, current_mask);
    
    //apply cleaning kernel
    cv::threshold(current_mask, current_mask, 10, 255, 0);
    cv::GaussianBlur(current_mask, current_mask, cv::Size(5, 5), 4, 4);
    
    return current_mask;
}

cv::VideoCapture parse_video(char* filename){

    cv::VideoCapture capture(filename);
    if(!capture.isOpened()){
        //error in opening the video input
        std::cerr << "Unable to open video file: " << filename << std::endl;
        std::exit(1);
    }
    return capture;
    
}

cv::Mat next_frame(cv::VideoCapture cap){
    cv::Mat frame;
    //read the current frame
    if(!cap.read(frame)) {
        std::cerr << "Unable to read next frame." << std::endl;
        std::cerr << "Video Finished: Exiting." << std::endl;
        std::exit(0);
    }
    
    return frame;
}



void processVideo(char* videoFilename) {
    
    int keyboard; //input from keyboard

    cv::Mat frame;   // video frame
    cv::Mat fg_mask; // masked frame
    cv::Mat fg_img;  // current mask applied to frame
    
    // backgroundsubtractor
    cv::Ptr<cv::BackgroundSubtractor> pKNN = cv::createBackgroundSubtractorKNN();
   
    // parse video
    cv::VideoCapture video = parse_video(videoFilename);
    
    int out_index;

    //read input data. ESC or 'q' for quitting
    while( (char)keyboard != 'q' && (char)keyboard != 27 ){
        
        frame = next_frame(video);
       
        cv::Size frame_dim = frame.size();
        cv::Rect roi(frame_dim.width / 2, 0, 50, frame_dim.height);
        cv::Mat image_roi;
        
        fg_mask = filter(pKNN, frame, fg_mask);

        
        // place region of interest
        fg_img = cv::Scalar::all(0);
        frame.copyTo(fg_img, fg_mask);
        image_roi = fg_img(roi);
        
        add_frame_number(video, frame);
        
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
    video.release();
}
