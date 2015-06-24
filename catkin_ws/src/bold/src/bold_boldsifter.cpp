#include "bold_boldsifter.hpp"

using namespace std;
using namespace cv;

namespace BOLD{
  
  void BOLDSifter::train(BOLDDatum d){
    bold.addLabeledFeatureFromFile(d);
    sift.train(d);
  }
  
  BOLDDatum BOLDSifter::classify(BOLDDatum d){
    BOLDDatum boldResult = bold.classify(d);
    BOLDDatum boldsiftResult = sift.classify(d,true,bold.getSortedLabels());
    if(boldsiftResult.label==(string)"")
      return boldResult;
    return boldsiftResult;
  }
  
  void BOLDSifter::clear(){
   bold.clear();
   sift.clear();
  }
}