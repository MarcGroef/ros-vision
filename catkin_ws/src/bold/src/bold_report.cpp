#include "bold_report.hpp"


using namespace std;


namespace BOLD{
  //**********************************************************BOLDImageReport*********************************************************
  BOLDImageReport::BOLDImageReport(BOLDDatum datum){
    this->datum = datum;
    nCorrect = 0;
    nFalse = 0;
    nTested = 0; 
  }
  
  void BOLDImageReport::update(bool correct,BOLDDatum datum){
    nTested++;
    if(correct)
      nCorrect++;
    else
      nFalse++;
    
    if(!correct)
      falseData.push_back(datum);
  }
  
  string BOLDImageReport::getLabel(){
    return datum.label;
  }
  
  int BOLDImageReport::getCorrect(){
    return nCorrect;
  }
  
  int BOLDImageReport::getFalse(){
    return nFalse;
  }
  
  //*************************************************************BOLDLabelReport********************************************************
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
      if(imageReports[i].getLabel().compare(label)==0)
	return i;
    }
    BOLDDatum datum(fileDir,label);
    BOLDImageReport rep(datum);
    imageReports.push_back(rep);
    return imageReports.size()-1;
  }
  
  void BOLDLabelReport::update(BOLDDatum datum,bool correct,BOLDDatum falseDatum){
    nTested++;
    if(correct)
      nCorrect++;
    else
      nFalse++;
    imageReports[fetchImageIndex(datum.filename)].update(correct,falseDatum);
    
  }
  
  void BOLDLabelReport::writeHTML(string dir){
    ofstream report;
    report.open((dir +"/"+label+".html").c_str());
    report << "<!DOCTYPE html>\n";
    report << "<html>\n";
    report << label << "\n";
    report << "</html>"; 
  }
  
  int BOLDLabelReport::getTotal(){
   return nTested; 
  }
  
  int BOLDLabelReport::getCorrect(){
    return nCorrect;
  }
  
  int BOLDLabelReport::getFalse(){
    return nFalse;
  }
  //********************************************************BOLDReport***************************************************************************
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
    BOLDLabelReport rep(lab);
    labelReports.push_back(rep);
    return labelReports.size()-1;
  }
  
  void BOLDReport::update(BOLDDatum datum,bool correct,BOLDDatum falseDatum){
    nTested++;
    if(correct)
      nCorrect++;
    else
      nFalse++;
    labelReports[fetchLabelReportIndex(datum.label)].update(datum,correct,falseDatum);
  }
  
  void BOLDReport::writeHTML(string reportName){
    ofstream report;
    
    
    if(!boost::filesystem::exists("Reports"))boost::filesystem::create_directory("Reports");
    mainDir = "Reports/" +reportName + "/";
    if(!boost::filesystem::exists(mainDir))boost::filesystem::create_directory(mainDir);
    report.open((mainDir +"Index.html").c_str());
    if(report.is_open()) std::cout << "open!\n";
    report << "<!DOCTYPE html>\n";
    report << "<html>\n";
    
    report << "<h1>BOLD crossfold report</h1>";
    report << "<p>Labels and within classification correctness</p>\n";
    for(size_t i=0;i<labelReports.size();i++){
      if(!boost::filesystem::exists(mainDir+labelReports[i].getLabel()))boost::filesystem::create_directory(mainDir+labelReports[i].getLabel());
      report << "<a href=\""<<  labelReports[i].getLabel() << "/" << labelReports[i].getLabel()<< ".html" << "\"> " << labelReports[i].getLabel() << "</a>\n";
      report << labelReports[i].getCorrect() << "/" << labelReports[i].getTotal() << " correct and " << labelReports[i].getFalse() << "/" << labelReports[i].getTotal() << "false. <br>\n";
      labelReports[i].writeHTML(mainDir + labelReports[i].getLabel()+"/");
    }    
    report << "</html>\n";
    
    report.close();
  }
  
}