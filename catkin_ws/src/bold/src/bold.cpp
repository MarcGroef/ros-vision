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
   histBinSize = 2*M_PI/HISTOGRAM_SIZE;
   entries = 0;
   highestCount=0;
  }
  

  
  void BOLDFeature::add(double alpha,double beta){
    histogram[(int)(alpha/histBinSize)][(int)(beta/histBinSize)]++;
    if(histogram[(int)(alpha/histBinSize)][(int)(beta/histBinSize)]>highestCount)
      highestCount = histogram[(int)(alpha/histBinSize)][(int)(beta/histBinSize)];
    entries++;
  }
  
  void BOLDFeature::print(){
    cout << "BOLD feature 2D Histogram with " << entries << " entries:\n";
    
    for(int i=0;i<HISTOGRAM_SIZE;i++){
     for(int j=0;j<HISTOGRAM_SIZE;j++){
       cout << histogram[i][j] << ", ";
     }
     cout << "\n";
    }
     
  }
  
  void BOLDFeature::show(int windowSize){
    Mat image(windowSize,windowSize,CV_8UC1);
    
    
    int imBlockSize = windowSize/HISTOGRAM_SIZE;
    for(int i=0;i<windowSize;i++){
     for(int j=0;j<windowSize;j++) {
       image.data[i+j*windowSize] =(uchar) (256*histogram[i/imBlockSize][j/imBlockSize]/highestCount);
     }
    }
    cv::namedWindow("BOLD feature:",cv::WINDOW_AUTOSIZE);
    imshow("BOLD feature:",image);
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
    cv::namedWindow("BOLD input");
    imshow("BOLD input",im);
    image = char_to_image_double_ptr(im.cols,im.rows,(char*)im.data);
    imWidth = im.cols;
    imHeight = im.rows;
    imageIsSet = true;
    
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
    showLines();
 
  }
  
  void BOLDescriptor::showLines(){
   Mat image(imWidth,imHeight,CV_8UC1,Scalar::all(256));
   
   for(int i=0;i<nLines;i++){
     cv::Point a(lines[i].x1,lines[i].y1),b(lines[i].x2,lines[i].y2);
    line(image,a,b,Scalar(0,0,0),1,8) ;
   }
   cv::namedWindow("LSD lines:",cv::WINDOW_AUTOSIZE);
    imshow("LSD lines:",image);
  }
    
  void BOLDescriptor::describe(){
    int i,j;

    double alpha,beta,signSI;
    BVector gmi,gmj,ei1,ei2,ej1,ej2,n(0,0,1),si,sj,mj,mi,tij,tji,signPart,st;
 
    
    if(lines==NULL){
      if(!imageIsSet){
	cout << "BOLD::BOLDescriptor.describe() error: image not set.\n";
	exit(-1);
      }else
	detectLines();
    }
    
    
    //for all individual line combinations :: TODO should be K-nearest-line combos
    for(i=0;i<nLines;i++){
      for(j=i+1;j<nLines;j++){
	//following variable namings from BOLD paper by Tombari e.a.
	
	// set vectors
	
	mi.set((lines[i].x1+lines[i].x2)/2,(lines[i].y1+lines[i].y2)/2,0);
	mj.set((lines[j].x1+lines[j].x2)/2,(lines[j].y1+lines[j].y2)/2,0);
	tij.set(mj.minus2D(mi));
	tji.set(mi.minus2D(mj));
	gmi.set(getGradient((int)((lines[i].x1+lines[i].x2))/2,(int)((lines[i].y1+lines[i].y2)/2)));  
	gmj.set(getGradient((int)((lines[j].x1+lines[j].x2))/2,(int)((lines[j].y1+lines[j].y2)/2)));	 
	ei1.set(lines[i].x1,lines[i].y1,0);
	ei2.set(lines[i].x2,lines[i].y2,0);
	ej1.set(lines[j].x1,lines[j].y1,0);
	ej2.set(lines[j].x2,lines[j].y2,0);
	
	
	//calculate sign(si) and sign(sj) and invert vector if nessesary
	signPart.set((ei2.minus2D(ei1)).cross(gmi));
	signSI = n.dot(signPart.divByScalar(signPart.abs()));
	
	si.set(ei2.minus2D(ei1).timesScalar(signSI));
	
	signPart.set((ej2.minus2D(ej1)).cross(gmj));
	signSI = n.dot(signPart.divByScalar(signPart.abs()));
	
	sj.set(ej2.minus2D(ej1).timesScalar(signSI));
	
	//calculate angles
	
	alpha = acos(si.dot(tij)/(si.abs()*tij.abs()));
	beta = acos(sj.dot(tji)/(sj.abs()*tji.abs()));
	
	//angle correction
	st = si.cross(tij);
	alpha = n.dot(st.divByScalar(st.abs()))==1?alpha:2*M_PI-alpha;
	st = sj.cross(tji);
	beta = n.dot(st.divByScalar(st.abs()))==1?beta:2*M_PI-beta;
	
	
	feature.add(alpha,beta);
      }
    }
    feature.print();
    feature.show(200);
    waitKey(0);
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