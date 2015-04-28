#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    ros::init(argc,argv,"image_converter");
    cv::initModule_nonfree();
    
    const cv::Mat input = cv::imread(argv[1], 1); //Load as grayscale
    cv::SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(input, keypoints);
    // Add results to image and save.
    cv::Mat output;
    cv::drawKeypoints(input, keypoints, output);
    cv::imwrite("sift_result.jpg", output);

    
    
    return 0;
}

