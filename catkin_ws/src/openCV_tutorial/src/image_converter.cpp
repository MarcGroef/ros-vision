#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <opencv/cv.hpp>


#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

extern "C"{
   #include "lsd_1.6/lsd.h"
}


#include <vector>
#include <iostream>
#include <string>


//using namespace std;
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
    SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(input, keypoints);
    //printVector(keypoints);
    // Add results to image and save.
    cv::Mat output;
    cv::drawKeypoints(input, keypoints, output);
    cv::imwrite("sift_result.jpg", output);
}

void performLSD(){

  double * image;
  double * out;
  int x,y,i,j,n;
  int X = 128;  /* x image size */
  int Y = 128;  /* y image size */

  /* create a simple image: left half black, right half gray */
  image = (double *) malloc( X * Y * sizeof(double) );
  if( image == NULL )
    {
      fprintf(stderr,"error: not enough memory\n");
      exit(EXIT_FAILURE);
    }
  for(x=0;x<X;x++)
    for(y=0;y<Y;y++)
      image[x+y*X] = x<X/2 ? 0.0 : 64.0; /* image(x,y) */


  /* LSD call */
  out = lsd(&n,image,X,Y);


  /* print output */
  printf("%d line segments found:\n",n);
  for(i=0;i<n;i++)
    {
      for(j=0;j<7;j++)
        printf("%f ",out[7*i+j]);
      printf("\n");
    }

  /* free memory */
  free( (void *) image );
  free( (void *) out );
}


int main(int argc, char* argv[]){
    cv::Mat output;
    
    ros::init(argc,argv,"image_converter");
    cv::initModule_nonfree();
    const cv::Mat input = cv::imread(argv[1], 1); //Load as grayscale
    performSIFT(input);  
    performLSD();
    
    return 0;
}

