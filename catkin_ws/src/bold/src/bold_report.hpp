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
    string filename;
    int nCorrectBOLD;
    int nFalseBOLD;
    int nCorrectSIFT;
    int nFalseSIFT;
    int nTestedBOLD;
    int nTestedSIFT;
    int nCorrectBOLDSIFTER;
    int nFalseBOLDSIFTER;
    int nTestedBOLDSIFTER;
    vector<BOLDDatum> falseData;
  public:
    BOLDImageReport(BOLDDatum datum);
    void updateBOLD(bool correct, BOLDDatum falseDatum);
    void updateSIFT(bool correct, BOLDDatum falseDatum);
    void updateBOLDSIFTER(bool correct, BOLDDatum flaseDatum);
    void writeHTML(string dir);
    string getFileName();
    int getFalse();
    string getLabel();
    string getdir();
    int getTotalBOLD();
    int getTotalSIFT();
    BOLDDatum getDatum();
    int getBOLDFalse();
    int getBOLDCorrect();
    int getSIFTCorrect();
    int getSIFTFalse();
    int getBOLDSIFTERFalse();
    int getBOLDSIFTERCorrect();
    int getBOLDSITERTotal();
    vector<BOLDDatum> getFalses();
  };
  
  class BOLDLabelReport{
  private:
    string label;
    int nCorrectBOLD;
    int nFalseBOLD;
    int nCorrectSIFT;
    int nFalseSIFT;
    int nTestedBOLD;
    int nTestedSIFT;
    int nTestedBOLDSIFTER;
    int nCorrectBOLDSIFTER;
    int nFalseBOLDSIFTER;
    int nPictures;
    vector<BOLDImageReport> imageReports;
    
  public:
    BOLDLabelReport(string label);
    
    int fetchImageIndex(string fileDir);
    string getLabel();
    void updateBOLD(BOLDDatum datum, bool correct,BOLDDatum falseDatum);
    void updateSIFT(BOLDDatum datum, bool correct,BOLDDatum falseDatum);
    void updateBOLDSIFTER(BOLDDatum datum,bool correct,BOLDDatum falseDatum);
    int getBOLDFalse();
    int getBOLDCorrect();
    int getSIFTCorrect();
    int getSIFTFalse();
    int getTotalBOLD();
    int getTotalSIFT();
    int getTotalBOLDSIFTER();
    int getBOLDSIFTERCorrect();
    int getBOLDSIFTERFalse();
    void writeHTML(string dir);
  };
  
  class BOLDReport{
  private:
    int nCorrectBOLD;
    int nFalseBOLD;
    int nCorrectSIFT;
    int nFalseSIFT;
    int nTestedBOLD;
    int nTestedSIFT;
    int nTestedBOLDSIFTER;
    int nCorrectBOLDSIFTER;
    int nFalseBOLDSIFTER;
    string mainDir;
    vector<BOLDLabelReport> labelReports;
    int fetchLabelReportIndex(string lab);

    
  public:
    BOLDReport();
    void updateBOLD(BOLDDatum datum,bool correct,BOLDDatum falseDatum);
    void updateSIFT(BOLDDatum datum,bool correct,BOLDDatum falseDatum);
    void writeHTML(string raportName);
  };
 
  
  
}


#endif
