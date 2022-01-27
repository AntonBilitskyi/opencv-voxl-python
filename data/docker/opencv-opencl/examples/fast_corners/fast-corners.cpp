#include <iostream>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>
#include <vector>
#include <chrono>

int cvFast(cv::Mat image, bool useGPU)
{
    std::vector<cv::KeyPoint> keypoints;
     
    if (useGPU) {
        cv::UMat uimage;
        image.copyTo(uimage);
        
        cv::FAST(uimage, keypoints, 20, true);
    } else {
        cv::FAST(image, keypoints, 20, true);
    }
  
    return keypoints.size();
}

int main(int argc, char *argv[])
{
    (void)argv;
    (void)argc;
    int keypoints;

    cv::Mat image = cv::imread("walking.jpeg", cv::IMREAD_UNCHANGED);

    cv::ocl::setUseOpenCL(true);
    if (!cv::ocl::useOpenCL()) {
        std::cerr << "Enable OpenCL failed!\n";
        return -1;
    }
    
    if (image.empty()) {
        std::cerr << "Open image 0.png failed" << std::endl;
        return -1;
    }

    auto start = std::chrono::steady_clock::now();

    for( int i=0; i<1000; i++ )
    {
        keypoints = cvFast(image, false);
    }

    auto end =std::chrono::steady_clock::now();
    std::chrono::duration<double> diff1 = end - start;

    printf("FAST (%4d x %4d), keypoints:%4zu, time:%6f cpu\n",
            image.cols, image.rows,
            keypoints,
            diff1.count());

    start = std::chrono::steady_clock::now();

    for( int i=0; i<1000; i++ )
    {
        keypoints = cvFast(image, true);
    }

    end =std::chrono::steady_clock::now();
    diff1 = end - start;

    printf("FAST (%4d x %4d), keypoints:%4zu, time:%6f gpu\n",
            image.cols, image.rows,
            keypoints,
            diff1.count());
    
    return 0;

}
