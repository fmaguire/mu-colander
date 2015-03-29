#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <stdio.h>

#include <iostream>

#include "utils.hpp"

cv::Mat filter(cv::Ptr<cv::BackgroundSubtractor> subtractor,
               cv::Mat current_frame, 
               cv::Mat current_mask){

    //update the background model
    subtractor->apply(current_frame, current_mask);
    
    //apply cleaning kernel
    //cv::threshold(current_mask, current_mask, 10, 255, 0);
    //cv::GaussianBlur(current_mask, current_mask, cv::Size(5, 5), 4, 4);
    
    return current_mask;
}

cv::Mat grey_scale(cv::Mat colour_frame){
    cv::Mat grey_frame;
    cv::cvtColor(colour_frame, grey_frame, cv::COLOR_BGR2GRAY);
    return grey_frame;
}


cv::Mat blur(cv::Mat frame){
    cv::Mat blur_frame;
    cv::GaussianBlur(frame, blur_frame, cv::Size(3, 3), 1, 1.5);
    return blur_frame;
}

cv::Mat subtract_frame(cv::Mat bg_frame,
                 cv::Mat current_frame){
      
    cv::Mat current_mask;
    cv::absdiff(bg_frame, current_frame, current_mask);
    
    return current_mask;
}


void processVideo(char* videoFilename) {
    
    int keyboard; //input from keyboard

    cv::Mat frame;   // video frame
    cv::Mat fg_mask; // masked frame
    cv::Mat fg_img;  // current mask applied to frame
    cv::Mat bg_frame;  // current mask applied to frame
    
    // backgroundsubtractor
    cv::Ptr<cv::BackgroundSubtractor> pKNN = cv::createBackgroundSubtractorKNN();
   
    // parse video
    cv::VideoCapture video = parse_video(videoFilename);
    
    //int out_index;
    bool first = true;

    //read input data. ESC or 'q' for quitting
    while( (char)keyboard != 'q' && (char)keyboard != 27 ){
        
        // get next frame or exit if end of video
        frame = next_frame(video);
        
        // convert to greyscale
        frame = grey_scale(frame);

       
        if( first ) {
            bg_frame = frame;
            first = false;
        }

        // filter frame
        fg_mask = filter(pKNN, frame, fg_mask);
        
        // add blur
        fg_mask = blur(fg_mask);
        
        // intialise region of interest objects
        // ROI is a rectangle 50 pixels wide in the middle of the frame
        cv::Size frame_dim = frame.size();
        cv::Rect roi(frame_dim.width / 2, 0, 50, frame_dim.height);
        cv::Mat image_roi;
        
        // apply region of interest on top of masked foreground image
        fg_img = cv::Scalar::all(0);
        frame.copyTo(fg_img, fg_mask);
        image_roi = fg_img(roi);
        
        // annotate frame with current frame_number
        add_frame_number(video, frame);
        
        // Display the current frame and the fg masks
        cv::imshow("frame", frame);
        //cv::imshow("fg_img", fg_img);
        cv::imshow("fg_mask", fg_mask);
        cv::imshow("roi", image_roi);

        // If mask in region of interest isn't all black
        // output images
        /*
        int presence = cv::sum(cv::sum(image_roi))[0];
        if (presence > 0){
            std::ostringstream fn;
            fn << "out/image_" << out_index << ".png";
            cv::imwrite(fn.str(), image_roi);
            out_index++;
        }
        */

        // Wait for keyboard and pause playback if period is pressed 
        keyboard = cv::waitKey(30);
        if ( (char)keyboard == '.' ) {
            keyboard = cv::waitKey(0);
        }
    }


    //delete capture object
    video.release();
}

