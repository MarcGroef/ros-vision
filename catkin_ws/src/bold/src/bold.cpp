#include "bold.hpp"


extern "C"{
  #include "lsd_1.6/lsd.h"
  #include "image_ext/image_ext.h"  
}


using namespace std;
using namespace cv;
using namespace BOLD;

namespace BOLD{
 

  
  
  
  //BOLD Feature functions
  
  BOLDFeature::BOLDFeature(){
   for(int i=0;i<HISTOGRAM_SIZE;i++) {
    for(int j=0;j<HISTOGRAM_SIZE;j++) {
      histogram[i][j]=0;
    }
   }
  }
  
  
  //BOLD Descriptor functions*/
  
  
  //Constructor
  BOLDescriptor::BOLDescriptor(){
    lines = NULL;
    imageIsSet = false;
    
    
  }
  
  BOLDescriptor::~BOLDescriptor(){
   free(image); 
  }
  
  
  BVector BOLDescriptor::getGradient(int x,int y){
    double gx = getImValue(x-1,y)-getImValue(x+1,y);
    double gy = getImValue(x,y-1)-getImValue(x,y+1);
    BVector g(gx,gy,0);
    return g;
  }
  
  double BOLDescriptor::getImValue(int x,int y){
    return image[x+y*imWidth];
  }
  
  //load an image into the BOLDescriptor
  void BOLDescriptor::setImage(Mat im){
    image = char_to_image_double_ptr(im.cols,im.rows,(char*)im.data);
    imWidth = im.cols;
    imHeight = im.rows;
    imageIsSet = true;
    histBinSize = M_PI/HISTOGRAM_SIZE;
  }
  
  //perform the LSD line detection from code of -von Gioi - e.a
  //Currently the line segments are written to a .eps image.
  
  void BOLDescriptor::detectLines(){


  
    // LSD call 
    lines = (Line*)lsd_scale(&nLines,image,imWidth,imHeight,1.0);

    cout << nLines << " line segments found\n";
    write_eps((double*)lines,nLines,7,(char*)"BOLDLSDout.eps",imWidth,imHeight,.1);
    cout << "Line image written to BOLDLSDout.eps..\n";
    // free memory 

 
  }
    
  void BOLDescriptor::describe(){
    int i,j;

    double alpha,beta;
    BVector gi,gj;
    int meanX,meanY;
    
    if(lines==NULL){
      if(!imageIsSet){
	cout << "BOLD::BOLDescriptor.describe() error: image not set.\n";
	exit(-1);
      }else
	detectLines();
    }
    
    for(i=0;i<nLines;i++){
      for(j=i+1;j<nLines;j++){
	//following variable namings from BOLD paper by Tombari e.a.
	gi = getGradient((int)((lines[i].x1+lines[i].x2))/2,(int)((lines[i].y1+lines[i].y2)/2));  
	gj = getGradient((int)((lines[j].x1+lines[j].x2))/2,(int)((lines[j].y1+lines[j].y2)/2));	 
	double gti = sqrt(gi.getElement(0)*gi.getElement(0)+gi.getElement(1)*gi.getElement(1));
	double gtj = sqrt(gj.getElement(0)*gj.getElement(0)+gj.getElement(1)*gj.getElement(1));
	cout << gti << " , " << gtj << "\n";
      }
    }
    
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