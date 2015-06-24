#include "bold_report.hpp"


using namespace std;


namespace BOLD{
  //**********************************************************BOLDImageReport*********************************************************
  BOLDImageReport::BOLDImageReport(BOLDDatum datum){
    this->datum = datum;
    nCorrectBOLD = 0;
    nFalseBOLD = 0;
    nCorrectSIFT = 0;
    nFalseSIFT = 0;
    nTestedBOLD = 0; 
    nTestedSIFT = 0;
    filename = getFileName();
  }
  
  void BOLDImageReport::updateBOLD(bool correct,BOLDDatum datum){
   nTestedBOLD++;
    if(correct)
      nCorrectBOLD++;
    else
      nFalseBOLD++;
    
    if(!correct)
      falseData.push_back(datum);
  }
  
  void BOLDImageReport::updateSIFT(bool correct,BOLDDatum datum){
   nTestedSIFT++;
    if(correct)
      nCorrectSIFT++;
    else
      nFalseSIFT++;
    
    if(!correct)
      falseData.push_back(datum);
  }

  void BOLDImageReport::updateBOLDSIFTER(bool correct,BOLDDatum datum){
    nTestedBOLDSIFTER++;
    if(correct)
     nCorrectBOLDSIFTER++;
    else
     nFalseBOLDSIFTER++;
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
    report << "<p> BOLD: " <<nCorrectBOLD << "/" << nTestedBOLD << " correct and " << nFalseBOLD << "/" <<  nTestedBOLD << "false</p>\n";
    report << "<p> SIFT: " <<nCorrectSIFT << "/" << nTestedSIFT << " correct and " << nFalseSIFT << "/" <<  nTestedSIFT << "false</p><br>\n";
    report << "<p> BOLDSIFTER: " << nCorrectBOLDSIFTER << "/" << nTestedBOLDSIFTER  << nFalseBOLDSIFTER << "/" << nTestedBOLDSIFTER << "false</p><br>\n";
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
  
  int BOLDImageReport::getBOLDCorrect(){
    return nCorrectBOLD;
  }
  
  int BOLDImageReport::getBOLDFalse(){
    return nFalseBOLD;
  }
  
  int BOLDImageReport::getSIFTCorrect(){
    return nCorrectSIFT;
  }
  
  int BOLDImageReport::getSIFTFalse(){
    return nFalseSIFT;
  }
  
  int BOLDImageReport::getTotalBOLD(){
    return nTestedBOLD;
  }
  
  int BOLDImageReport::getTotalSIFT(){
    return nTestedSIFT;
  }
  
  BOLDDatum BOLDImageReport::getDatum(){
    return datum;
  }

  
  
  //*************************************************************BOLDLabelReport********************************************************
  BOLDLabelReport::BOLDLabelReport(string label){
    this->label = label;
    nCorrectBOLD = 0;
    nCorrectSIFT = 0;
    nFalseBOLD = 0;
    nFalseSIFT = 0;
    nTestedBOLD = 0;
    nTestedSIFT = 0;
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
  
  void BOLDLabelReport::updateBOLD(BOLDDatum datum,bool correct,BOLDDatum falseDatum){
    nTestedBOLD++;
    if(correct)
      nCorrectBOLD++;
    else
      nFalseBOLD++;
    imageReports[fetchImageIndex(datum.filename)].updateBOLD(correct,falseDatum);
    
  }
  
  void BOLDLabelReport::updateSIFT(BOLDDatum datum,bool correct,BOLDDatum falseDatum){
    nTestedSIFT++;
    if(correct)
      nCorrectSIFT++;
    else
      nFalseSIFT++;
    imageReports[fetchImageIndex(datum.filename)].updateSIFT(correct,falseDatum);
    
  }
  
  void BOLDLabelReport::writeHTML(string dir){
    ofstream report;
    report.open((dir +"/"+label+".html").c_str());
    report << "<!DOCTYPE html>\n";
    report << "<html>\n";
    report << "<h1> " << label << "</h1>" "\n";
    report << "<img src = \"../../../" << imageReports[0].getDatum().filename << "\" alt = \"" << imageReports[0].getDatum().filename << "\" style= \" width:320px;height:240px;\"> ";
    report << "<p> BOLD: " <<nCorrectBOLD << "/" << nTestedBOLD << " correct and " << nFalseBOLD << "/" <<  nTestedBOLD << "false</p>\n";
    report << "<p> SIFT: " <<nCorrectSIFT << "/" << nTestedSIFT << " correct and " << nFalseSIFT << "/" <<  nTestedSIFT << "false</p><br>\n";
    for(int i=0;i<imageReports.size();i++){
      report << "<a href=\""<<  imageReports[i].getdir() << "\"> " << imageReports[i].getdir() << "</a><br>\n";
      report << "BOLD: " << imageReports[i].getBOLDCorrect() << "/" << imageReports[i].getTotalBOLD() << " correct and " << imageReports[i].getBOLDFalse() << "/" <<  imageReports[i].getTotalBOLD() << "false<br>\n";
      report << "SIFT: " << imageReports[i].getSIFTCorrect() << "/" << imageReports[i].getTotalSIFT() << " correct and " << imageReports[i].getSIFTFalse() << "/" <<  imageReports[i].getTotalSIFT() << "false<br>\n";
      
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
  
  int BOLDLabelReport::getTotalBOLD(){
   return nTestedBOLD; 
  }
  
  int BOLDLabelReport::getTotalSIFT(){
    return nTestedSIFT;
  }
  
  int BOLDLabelReport::getBOLDCorrect(){
    return nCorrectBOLD;
  }
  
  int BOLDLabelReport::getSIFTCorrect(){
    return nCorrectSIFT;
  }
  
  int BOLDLabelReport::getBOLDFalse(){
    return nFalseBOLD;
  }
  
  int BOLDLabelReport::getSIFTFalse(){
    return nFalseSIFT;
  }
  //********************************************************BOLDReport***************************************************************************
  BOLDReport::BOLDReport(){
    nCorrectBOLD = 0;
    nCorrectSIFT = 0;
    nFalseBOLD = 0;
    nFalseSIFT = 0;
    nTestedBOLD = 0;
    nTestedSIFT = 0;
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
  
  void BOLDReport::updateBOLD(BOLDDatum datum,bool correct,BOLDDatum falseDatum){
    nTestedBOLD++;
    if(correct)
      nCorrectBOLD++;
    else
      nFalseBOLD++;
    labelReports[fetchLabelReportIndex(datum.label)].updateBOLD(datum,correct,falseDatum);
  }
  
  void BOLDReport::updateSIFT(BOLDDatum datum,bool correct,BOLDDatum falseDatum){
    nTestedSIFT++;
    if(correct)
      nCorrectSIFT++;
    else
      nFalseSIFT++;
    labelReports[fetchLabelReportIndex(datum.label)].updateSIFT(datum,correct,falseDatum);
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
    report << "<p> BOLD: " <<nCorrectBOLD << "/" << nTestedBOLD << " correct and " << nFalseBOLD << "/" <<  nTestedBOLD << "false</p>\n";
    report << "<p> SIFT: " <<nCorrectSIFT << "/" << nTestedSIFT << " correct and " << nFalseSIFT << "/" <<  nTestedSIFT << "false</p><br>\n";
    for(size_t i=0;i<labelReports.size();i++){
      if(!boost::filesystem::exists(mainDir+labelReports[i].getLabel()))boost::filesystem::create_directory(mainDir+labelReports[i].getLabel());
      report << "<a href=\""<<  labelReports[i].getLabel() << "/" << labelReports[i].getLabel()<< ".html" << "\"> " << labelReports[i].getLabel() << "</a><br>\n";
      report << "BOLD: "<<labelReports[i].getBOLDCorrect() << "/" << labelReports[i].getTotalBOLD() << " correct and " << labelReports[i].getBOLDFalse() << "/" << labelReports[i].getTotalBOLD() << "false. <br>\n";
      report << "SIFT: "<<labelReports[i].getSIFTCorrect() << "/" << labelReports[i].getTotalSIFT() << " correct and " << labelReports[i].getSIFTFalse() << "/" << labelReports[i].getTotalSIFT() << "false. <br><br>\n";
      labelReports[i].writeHTML(mainDir + labelReports[i].getLabel()+"/");
    }    
    report << "</html>\n";
    
    report.close();
    cout << "Written that shit!\n";
  }
  
}
