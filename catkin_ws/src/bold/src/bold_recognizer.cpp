#include "bold_recognizer.hpp"

using namespace std;
using namespace cv;

namespace BOLD{
  
  BOLDRecognizer::BOLDRecognizer(){
    for(int i=0;i<K_NEAREST_NEIGHBORS;i++)
      kNeas
  }
  
  string BOLDRecognizer::classify(BOLDFeature f){
    int nTrainedElements = trainedFeatures.size();
    int kNearestNeighborIndices[K_NEAREST_NEIGHBORS];
    double distances[K_NEAREST_NEIGHBORS];
    
    double dist;
    double buffer;
    int buff2,buff3;
    
    for(int i=0;i<K_NEAREST_NEIGHBORS;i++)
      distances = DBL_MAX;
    
    for(int i=0;i<nTrainedElements;i++){
      dist = f.getDistance(trainedFeatures.at(i));
      for(int j=0;j<K_NEAREST_NEIGHBORS;j++){
	if(dist<=distances[d]){
	  buff2 = d;
	  for(int n=d;n<K_NEAREST_NEIGHBORS;n++){
	    buffer = distances[d];
	    distances[n] = dist;
	    dist = buffer;
	    buff3 = kNearestNeighborIndices[n];
	    kNearestNeighborIndices[n]=buff2;
	    buff2 = buff3;
	  }
	  
	}
      }
    }
    
    
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
   
    
  }
  
}