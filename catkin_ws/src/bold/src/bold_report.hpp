#ifndef BOLD_REPORT_HPP
#define BOLD_REPORT_HPP


#include <string>
#include <fstream>
#include <sstream>

#include <vector>


using namespace std;

namespace BOLD{
  
  class BOLDImageReport{
  private:
    string label;
    string filedir;
    int nCorrect;
    int nFalse;
    int nTested;
    vector<string> falsedirs;
  public:
    BOLDImageReport(string label,string filedir);
    update(bool correct, string falseDir);
  }
  
  class BOLDLabelReport{
  private:
    string label;
    int nCorrect;
    int nFalse;
    int nTested;
    int nPictures;
    vector<BOLDImageReport> imageReports;
    
  public:
    BOLDLabelReport(string label);
    string getLabel();
    void update(string filedir, bool correct,string falseDir);
  }
  
  class BOLDReport{
  private:
    int nCorrect;
    int nFalse;
    int nTested;
    vector<BOLDLabelReport> labelReports;
    BOLDLabelReport fetchLabelReportIndex(string lab);
  public:
    BOLDReport();
    void update(string label,string filedir,bool correct,string falseLabel,string falseDir);
  }
 
  
  
}


#endif