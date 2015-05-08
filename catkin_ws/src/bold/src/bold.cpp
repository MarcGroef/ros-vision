#include "bold.hpp"


extern "C"{
  #include "lsd_1.6/lsd.h"
  #include "image_ext/image_ext.h"  
}


using namespace std;
using namespace cv;
using namespace BOLD;

namespace BOLD{
  
  BOLDescriptor::BOLDescriptor(){
    cout << "BOLDescriptor contstructor called\n";
  }
  
  void BOLDescriptor::setImage(Mat im){
    image = im;
  }
  
  
    
  void BOLDescriptor::describe(){
    
    double * im = char_to_image_double_ptr(image.cols,image.rows,(char*)image.data);
    
    int x,y,i,j,n;
    int X = image.cols;  /* x image size */
    int Y = image.rows;  /* y image size */

  
    /* LSD call */
    bol = lsd(&n,im,X,Y);

    printf("%d line segments found:\n",n);
    /* free memory */
    write_eps(bol,n,7,(char*)"BOLDLSDout.eps",X,Y,.1);
    //free( (void *) image );
    free( (void *) im );
  }
  
  
 
  
}

int main(int argc,char**argv){
  ros::init(argc,argv,"bold");
  cv::initModule_nonfree();
  
  
  BOLDescriptor d;
  d.setImage(cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE));
  d.describe();
  return 0;
}