#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <opencv/cv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/line_descriptor/descriptor.hpp>
#include <iostream>



using namespace std;
using namespace cv;


void printSIFTVector(std::vector<cv::KeyPoint> kp){
     int i,size;
     size = kp.size();
     for(i=0;i<size;i++){
        std::cout << kp[i].pt << "\n";   
     }
     std::cout << "there were " << size << " features\n";
}

void performSIFT(cv::Mat input){
    cv::SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(input, keypoints);
    //printVector(keypoints);
    // Add results to image and save.
    cv::Mat output;
    cv::drawKeypoints(input, keypoints, output);
    cv::imwrite("sift_result.jpg", output);
}

/*void performLSD(cv::Mat input){
    
    std::vector<cv::line_descriptor::KeyLine> keylines;
    cv::Mat output;
    cv::LSDDetector detector;
    detector.detect(input,keylines,1,2);
   
}*/


int main(int argc, char* argv[]){
    cv::Mat output;
    
    ros::init(argc,argv,"image_converter");
    cv::initModule_nonfree();
    const cv::Mat input = cv::imread(argv[1], 1); //Load as grayscale
    performSIFT(input);  
    //performLSD(input);
    
    return 0;
}

