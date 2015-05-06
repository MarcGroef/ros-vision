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
    SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(input, keypoints);
    //printVector(keypoints);
    // Add results to image and save.
    cv::Mat output;
    cv::drawKeypoints(input, keypoints, output);
    cv::imwrite("sift_result.jpg", output);
}

double* char_to_image_double_ptr( unsigned int xsize, 
                                     unsigned int ysize, char * data ){

  int size = xsize*ysize;
  int i;
  double* image;
  /* check parameters */
  if( xsize == 0 || ysize == 0 )
    cout << "new_image_double_ptr: invalid image size.";
  if( data == NULL ) cout << "new_image_double_ptr: NULL data pointer.";

  /* get memory */
  image = (double*) malloc(size* sizeof(double) );
  if( image == NULL ) cout << "not enough memory.";

  /* set image */

  for(i=0; i < size; ++i){
    image[i] = (double) data[i];
  }
  return image;
}

void performLSD(cv::Mat m){
  
  double * image = char_to_image_double_ptr(m.cols,m.rows,(char*)m.data);
  double * out;
  int x,y,i,j,n;
  int X = m.cols;  /* x image size */
  int Y = m.rows;  /* y image size */

 
  /* LSD call */
  out = lsd(&n,image,X,Y);


  /* print output */

  for(i=0;i<n;i++)
    {
      for(j=0;j<7;j++)
        printf("%f ",out[7*i+j]);
      printf("\n");
    }
  printf("%d line segments found:\n",n);
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
    performLSD(input);
    
    return 0;
}

