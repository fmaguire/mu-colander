//opencv
#include <opencv2/highgui.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>

//local
#include "utils.hpp"
#include "processing.hpp"

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

