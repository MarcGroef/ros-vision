#include "bold_recognizer.hpp"

using namespace std;
using namespace cv;
using namespace BOLD;

namespace BOLD{
  
  BOLDRecognizer::BOLDRecognizer(){

  }
  
  string BOLDRecognizer::classify(BOLDFeature f){
    int nTrainedElements = trainedFeatures.size();
    int kNearestNeighborIndices[K_NEAREST_NEIGHBORS];
    double distances[K_NEAREST_NEIGHBORS];
    
    double dist;
    double buffer;
    int buff2,buff3;
    
    for(int i=0;i<K_NEAREST_NEIGHBORS;i++)
      distances[i] = DBL_MAX;
    
    for(int i=0;i<nTrainedElements;i++){
      dist = f.distanceFrom(trainedFeatures.at(i));
      for(int j=0;j<K_NEAREST_NEIGHBORS;j++){
	if(dist<=distances[j]){
	  buff2 = j;
	  for(int n=j;n<K_NEAREST_NEIGHBORS;n++){
	    buffer = distances[j];
	    distances[n] = dist;
	    dist = buffer;
	    buff3 = kNearestNeighborIndices[n];
	    kNearestNeighborIndices[n]=buff2;
	    buff2 = buff3;
	  }
	  
	}
      }
    }
    //TODO find out most common label
    
  }
  
  void BOLDRecognizer::addLabeledFeature(BOLDFeature f){
   if(f.getLabel()=="") {
    cout << "BOLD::BOLDRecognizer::addLabeledFeature(): WARNING! Trying to add labeled feature, but feature label is not set.. Feature not added.\n" ;
    return;
   }
   if(!f.isNormalized()){
    cout << "BOLD::BOLDRecognizer::addLabeledFeature(): WARNING! Trying to add labeled feature, but feature is not yet normalized.. Feature not added.\n" ;
    return;
   }
   
    trainedFeatures.push_back(f);
  }
  
  
  void BOLDRecognizer::addLabeledFeatureFromFile(string fileName,string label){
    descriptor.setImage(fileName,false);
    descriptor.describe();
    descriptor.setFeatureLabel(label);
    trainedFeatures.push_back(descriptor.getFeature());
    descriptor.clear();
    cout << "labeled feature "+label+ "from "+fileName+" has been added to the trainingset\n";
  }
  
}


int main(int argc,char**argv){
  ros::init(argc,argv,"bold");
  cv::initModule_nonfree();
  cout << argc << "\n";
  
  
  BOLDescriptor d;
  for(int i=1;i<argc;i++){
    cout << argv[i] << "\n";
    d.setImage(argv[i],false);
    d.setImageName(argv[i]);
    d.describe();
    d.showLines();
    d.showFeatures();
    d.clear();
    
  }
  waitKey(0);
  return 0;
}