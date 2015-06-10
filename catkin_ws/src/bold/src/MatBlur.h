#ifndef MATBLUR_H
#define MATBLUR_H

#include <ros/ros.h>
#include <image_transport/image_transport.h>

#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv/cv.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace BOLD{

class MatBlur
{
public:
  MatBlur(cv::Mat image);
  
  cv::Mat resize(double ratio);
  cv::Mat d_Mat;

};
  MatBlur::MatBlur(cv::Mat image): d_Mat(image)
  {  }
  
  cv::Mat MatBlur::GaussianBlur(Size ksize, double sigmaX, double sigmaY=0, int borderType=BORDER_DEFAULT)
  {
    cv::Mat tmp;
    cv::GaussianBlur(d_Mat,tmp,ksize,sigmaX,sigmaY,borderType);
    return tmp;
  }

  cv::Mat MatBlur::resize(double ratio){
    cv::Mat tmp;
    Size cursize = d_Mat.size();
    Size size(cursize.width*ratio,cursize.height*ratio);
    if(ratio == 0)
      cout << "Hier wordt Marc niet blij van.";
    else if(ratio < 1)
      cv::resize(d_Mat,tmp,size,0,0,CV_INTER_AREA);
    else
      cv::resize(d_Mat,tmp,size,0,0,CV_INTER_CUBIC);
    return tmp;
  }
  
}
#endif 