#include "bold_report.hpp"


using namespace std;


namespace BOLD{
  //BOLDImageReport*********************************************************
  BOLDImageReport::BOLDImageReport(string label,string filedir){
    this->label = label;
    this->filedir = filedir;
    nCorrect = 0;
    nFalse = 0;
    nTested = 0; 
  }
  
  void BOLDImageReport::update(bool correct,string falseDir){
    nTested++;
    if(correct)
      nCorrect++;
    else
      nFalse++;
    
    if(!correct)
      falsedirs.push_back(falseDir);
  }
  
  
  //BOLDLabelReport********************************************************
  BOLDLabelReport::BOLDLabelReport(string label){
    this->label = label;
    nCorrect = 0;
    nFalse = 0;
    nTested = 0;
  }
  
  string BOLDLabelReport::getLabel(){
    return label;
  }
  
  int BOLDLabelReport::fetchImageIndex(string fileDir){
    int arrSize = imageReports.size();
    for(int i=0;i<arrSize;i++){
      if(imageReports[i].getLabel().compare(lab)==0)
	return i;
    }
    imageReports rep(label);
    imageReports.push_back(rep);
    return imageReports.size()-1;
  }
  
  void BOLDLabelReport::update(string fileDir,bool correct,string falseDir){
    nTested++;
    if(correct)
      nCorrect++;
    else
      nFalse++;
    imageReports[fetchImageIndex(fileDir)].update(correct,falseDir);
    
  }
  
  //BOLDReport**************************************************************
  BOLDReport::BOLDReport(){
    nCorrect = 0;
    nFalse = 0;
    nTested = 0;
  }
  
  int BOLDReport::fetchLabelReportIndex(string lab){
    int arrSize = labelReports.size();
    for(int i=0;i<arrSize;i++){
      if(labelReports[i].getLabel().compare(lab)==0)
	return i;
    }
    BOLDLabelReport rep(label);
    labelReports.push_back(rep);
    return labelReports.size()-1;
  }
  
  BOLDReport::update(string label,string filedir,bool correct,string falseLabel,string falseDir){
    nTested++;
    if(correct)
      nCorrect++;
    else
      nFalse++;
    labelReports[fetchLabelReportIndex(label)].update(filedir,correct,falseLabel);
  }
  
}