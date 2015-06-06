#ifndef BOLD_REPORT_HPP
#define BOLD_REPORT_HPP


#include <string>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <vector>

#include "bold_datum.hpp"


using namespace std;

namespace BOLD{
  
  class BOLDImageReport{
  private:
    BOLDDatum datum;
    int nCorrect;
    int nFalse;
    int nTested;
    vector<BOLDDatum> falseData;
  public:
    BOLDImageReport(BOLDDatum datum);
    void update(bool correct, BOLDDatum falseDatum);
    int getFalse();
    string getLabel();
    int getCorrect();
  };
  
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
    
    int fetchImageIndex(string fileDir);
    string getLabel();
    void update(BOLDDatum datum, bool correct,BOLDDatum falseDatum);
    int getFalse();
    int getCorrect();
    int getTotal();
    void writeHTML(string dir);
  };
  
  class BOLDReport{
  private:
    int nCorrect;
    int nFalse;
    int nTested;
    string mainDir;
    vector<BOLDLabelReport> labelReports;
    int fetchLabelReportIndex(string lab);

    
  public:
    BOLDReport();
    void update(BOLDDatum datum,bool correct,BOLDDatum falseDatum);
    void writeHTML(string raportName);
  };
 
  
  
}


#endif