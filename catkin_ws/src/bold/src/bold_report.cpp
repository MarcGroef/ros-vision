#include "bold_report.hpp"


using namespace std;


namespace BOLD{
  //**********************************************************BOLDImageReport*********************************************************
  BOLDImageReport::BOLDImageReport(BOLDDatum datum){
    this->datum = datum;
    nCorrect = 0;
    nFalse = 0;
    nTested = 0; 
    filename = getFileName();
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
  
  string BOLDImageReport::getdir(){
    return filename;
  }
  
  vector<BOLDDatum> BOLDImageReport::getFalses(){
    return falseData;  
  }
  
  
  string BOLDImageReport::getFileName(){
    string name;
    int dirLen = datum.filename.length();
    size_t slashLoc = datum.filename.find_last_of("/");
    size_t dotLoc = datum.filename.find_last_of(".");
    name = datum.filename.substr(slashLoc+1,(dotLoc-slashLoc-1));
  
    return name+".html";
  }
  
  void BOLDImageReport::writeHTML(string dir){
    ofstream report;

    report.open((dir +"/"+filename).c_str());
    report << "<!DOCTYPE html>\n";
    report << "<html>\n";
    report << "<h1>" << filename.substr(0,filename.find_last_of(".")) << "</h1>\n";
    report << "<p>" <<nCorrect << "/" << nTested << " correct and " << nFalse << "/" <<  nTested << "false</p><br>\n";
    report << "<img src = \"../../../" << datum.filename << "\" alt = \"" << filename << "\" style= \" width:320px;height:240px;\"> ";
    
    report << "<h1> Falsely compared to: </h1><br>\n ";
    for(int i=0;i<falseData.size();i++){
      report << "<img src = \"../../../" << falseData[i].filename << "\" alt = \"" << falseData[i].filename << "\" style= \" width:320px;height:240px;\"> ";
    }
    report << "</html>"; 
    report.close();
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
  
  int BOLDImageReport::getTotal(){
    return nTested;
  }
  
  BOLDDatum BOLDImageReport::getDatum(){
    return datum;
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
      if(imageReports[i].getDatum().filename.compare(fileDir)==0)
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
    report << "<h1> " << label << "</h1>" "\n";
    report << "<img src = \"../../../" << imageReports[0].getDatum().filename << "\" alt = \"" << imageReports[0].getDatum().filename << "\" style= \" width:320px;height:240px;\"> ";
    report << "<p>" <<nCorrect << "/" << nTested << " correct and " << nFalse << "/" <<  nTested << "false</p><br>\n";
    for(int i=0;i<imageReports.size();i++){
      report << "<a href=\""<<  imageReports[i].getdir() << "\"> " << imageReports[i].getdir() << "</a><br>\n";
      report << imageReports[i].getCorrect() << "/" << imageReports[i].getTotal() << " correct and " << imageReports[i].getFalse() << "/" <<  imageReports[i].getTotal() << "false<br>\n";
      
      imageReports[i].writeHTML(dir);
    }
    
    report << "<h1> Falsely compared to: </h1><br>\n ";
    for(int i=0;i<imageReports.size();i++)
      for(int j=0;j<imageReports[i].getFalses().size();j++){
	report << "<img src = \"../../../" << imageReports[i].getFalses()[j].filename << "\" alt = \"" << imageReports[i].getFalses()[j].filename << "\" style= \" width:320px;height:240px;\"> ";
      }
    report << "</html>"; 
    report.close();
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
    report << "<p>" <<nCorrect << "/" << nTested << " correct and " << nFalse << "/" <<  nTested << "false</p><br>\n";
    for(size_t i=0;i<labelReports.size();i++){
      if(!boost::filesystem::exists(mainDir+labelReports[i].getLabel()))boost::filesystem::create_directory(mainDir+labelReports[i].getLabel());
      report << "<a href=\""<<  labelReports[i].getLabel() << "/" << labelReports[i].getLabel()<< ".html" << "\"> " << labelReports[i].getLabel() << "</a><br>\n";
      report << labelReports[i].getCorrect() << "/" << labelReports[i].getTotal() << " correct and " << labelReports[i].getFalse() << "/" << labelReports[i].getTotal() << "false. <br><br>\n";
      labelReports[i].writeHTML(mainDir + labelReports[i].getLabel()+"/");
    }    
    report << "</html>\n";
    
    report.close();
    cout << "Written that shit!\n";
  }
  
}