#include "bold.hpp"


extern "C"{
  #include "lsd_1.6/lsd.h"
  #include "image_ext/image_ext.h"  
}


using namespace std;
using namespace cv;
using namespace BOLD;

namespace BOLD{
 

  //BOLD Descriptor functions*/
  
  
  //Constructor
  BOLDescriptor::BOLDescriptor(){
    lines = NULL;
    imageIsSet = false;
    imageName = "";
    linesIsSet = false;
  }
  
  BOLDescriptor::~BOLDescriptor(){
   clear();
  }
  
  void BOLDescriptor::setImageName(string name){
   imageName = name; 
  }
  
  string BOLDescriptor::getImageName(){
   return imageName; 
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
  void BOLDescriptor::setImage(Mat im,bool showInput){
    if(showInput){
      cv::namedWindow("BOLD input",cv::WINDOW_AUTOSIZE);
      imshow("BOLD input",im);
    }
    image = char_to_image_double_ptr(im.cols,im.rows,(char*)im.data);
    imWidth = im.cols;
    imHeight = im.rows;
    imageIsSet = true;
    
  }
  
  void BOLDescriptor::setImage(string fileName,bool showInput){
    Mat im = cv::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
    imageName = fileName;
    if(showInput){
      cv::namedWindow("BOLD input",cv::WINDOW_AUTOSIZE);
      imshow("BOLD input",im);
    }
    image = char_to_image_double_ptr(im.cols,im.rows,(char*)im.data);
    imWidth = im.cols;
    imHeight = im.rows;
    imageIsSet = true;
    
  }
  
  void BOLDescriptor::showFeatures(){
   feature.show(imageName); 
  }
  
  //perform the LSD line detection from code of -von Gioi - e.a
  //Currently the line segments are written to a .eps image.
  
  void BOLDescriptor::detectLines(){


    lines = (Line*)lsd_scale(&nLines,image,imWidth,imHeight,1.0);
    cout << nLines << " line segments found\n";
    linesIsSet = true;
    //showLines();
  }
  
  void BOLDescriptor::showLines(){
    Mat image(imHeight,imWidth,CV_8UC1,Scalar::all(256));
   
    for(int i=0;i<nLines;i++){
      cv::Point a(lines[i].x1,lines[i].y1),b(lines[i].x2,lines[i].y2);
    line(image,a,b,Scalar(0,0,0),1,8) ;
    }
    cv::namedWindow("LSD lines: "+imageName,cv::WINDOW_AUTOSIZE);
    imshow("LSD lines: "+imageName,image);
    cout << "lines should be shown..\n";
    waitKey(0);
    cv::destroyWindow("LSD lines: "+imageName);
    cv::destroyAllWindows();
    waitKey(1);
  }

  bool checkFalseLine(std::vector<int> v,int index){
    for (int i = 0; i < v.size();i++){
     if(index==v.at(i)) 
       return true;
    }
    return false;
  }
  
  void BOLDescriptor::resolveAngles(int i,int j){
    if(i==j) return;
    double alpha,beta,signSI;
    BVector gmi,gmj,ei1,ei2,ej1,ej2,n(0,0,1),si,sj,mj,mi,tij,tji,signPart,st;
    
    if(checkFalseLine(falseLines,i)||checkFalseLine(falseLines,j))
      return;
    gmi.set(getGradient((int)((lines[i].x1+lines[i].x2))/2,(int)((lines[i].y1+lines[i].y2)/2)));
    gmj.set(getGradient((int)((lines[j].x1+lines[j].x2))/2,(int)((lines[j].y1+lines[j].y2)/2)));	 
    if(gmi.abs()==0){
      cout << "BOLD::BOLDescriptor::describe(): Warning! mid-line gradient was 0. Line omitted! \n" ;
      falseLines.push_back(i);
      return;
    }
    if(gmj.abs()==0){
	cout << "BOLD::BOLDescriptor::describe(): Warning! mid-line gradient was 0. Line omitted! \n" ;
	falseLines.push_back(j);
	return;
    }
    
    mi.set((lines[i].x1+lines[i].x2)/2,(lines[i].y1+lines[i].y2)/2,0);
    mj.set((lines[j].x1+lines[j].x2)/2,(lines[j].y1+lines[j].y2)/2,0);
    tij.set(mj.minus2D(mi));
    tji.set(mi.minus2D(mj));
      
    
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
    double frac = si.dot(tij)/(si.abs()*tij.abs());

    alpha = acos(frac);
    
    frac = sj.dot(tji)/(sj.abs()*tji.abs());
    beta = acos(sj.dot(tji)/(sj.abs()*tji.abs()));

    //angle correction
    st = si.cross(tij);
    alpha = n.dot(st.divByScalar(st.abs()))==1?alpha:2*M_PI-alpha;
    st = sj.cross(tji);
    beta = n.dot(st.divByScalar(st.abs()))==1?beta:2*M_PI-beta;
    
    if(std::isnan(alpha)){
      falseLines.push_back(i);
      cout << "BOLD::BOLDescriptor::describe(): Warning! alpha was NaN. Omitting line..\n";
      return;
    }
    if(std::isnan(beta)){
      falseLines.push_back(j);
      cout << "BOLD::BOLDescriptor::describe(): Warning! beta was NaN. Omitting line..\n";
      return;
    }

    feature.add(alpha,beta);
      
    
  }

  void BOLDescriptor::kNearestLines(int index){
    
    double distances[K_NEAREST_LINE_SEGMENTS];
    double mix,miy,mjx,mjy;
    double dist,buffer;
    int buff2,buff3;
    mix = (lines[index].x1+lines[index].x2)/2;
    miy = (lines[index].y1+lines[index].y2)/2;
    
    for(int i=0;i<K_NEAREST_LINE_SEGMENTS;i++){
      distances[i]=DBL_MAX;
      KNLIndices[i] = -1;
    }
    
    for(int j=0;j<nLines;j++){
      if (j==index)
	continue;
      mjx = (lines[j].x1+lines[j].x2)/2;
      mjy = (lines[j].y1+lines[j].y2)/2;
      
      dist = sqrt((mix-mjx)*(mix-mjx)+(miy-mjy)*(miy-mjy));
      
      for(int d=0;d<K_NEAREST_LINE_SEGMENTS;d++){
	if (dist<=distances[d]){
	  buff2=d;
	  for(int n=d;n<K_NEAREST_LINE_SEGMENTS;n++){
	   buffer = distances[n];
	   distances[n]=dist;
	   dist = buffer;
	   
	   buff3 = KNLIndices[n];
	   KNLIndices[n] = buff2;
	   buff2=buff3;
	  }
	}
      }
    }
    //cout << "closest: " << distances[0] << ", furthest: " << distances[K_NEAREST_LINE_SEGMENTS-1] << "\n";
  }
  
  
  
  void BOLDescriptor::describe(){
   int i,j;
   cout << "describing..\n" ;
   feature.clear();
   if(lines==NULL){
      if(!imageIsSet){
	cout << "BOLD::BOLDescriptor.describe() error: image not set.\n";
	exit(-1);
      }else
	detectLines();
    }
    
    for(i=0;i<nLines;i++){
      kNearestLines(i);
      for(j=0;j<K_NEAREST_LINE_SEGMENTS;j++){
	if(KNLIndices[j]<nLines)  //secures against situations when nLines < K
	  resolveAngles(i,KNLIndices[j]);
      }
    }
    cout << "normalize..\n";
    feature.normalize();

  }
  
  void BOLDescriptor::setFeatureLabel(string label){
    feature.setLabel(label);
  }
  
  BOLDFeature BOLDescriptor::getFeature(){
    return feature;
  }
  
  void BOLDescriptor::clear(){
   
    
    
    //if(imageIsSet){
      free(image);
      image = NULL;
  //  }
  //  if(linesIsSet){
      free(lines);
      lines = NULL;
  //  }
    
    feature.clear();
    
    
    imageIsSet = false;
    linesIsSet = false;
    falseLines.clear();
    imageName = "";
  }
  
 
  
}


