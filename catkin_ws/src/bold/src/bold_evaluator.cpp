#include "bold_evaluator.hpp"

#include <sstream>

using namespace std;

namespace BOLD{
  
 
  
  //firstN = 0 = ALL
  void BOLDEvaluator::readDataset(string mainDir,int firstN){
      
      mainDatasetDirectory = mainDir;
      
      std::ifstream input((mainDatasetDirectory+"info.txt").c_str(), std::ios::in);
      if(!input){
	cout << "error reading dataset..\n";
	if(input.eof()) cout << "end of file..";
	if(!input.is_open()) cout << "file not open..\n";
	input.close();
      }
      
      input >> nLabels;
     // cout << "scanning " << nLabels << " labels\n";
      
      for( int i = 0;i < nLabels ; i++){
	if(firstN!=0&&i==firstN-1)
	  break;
	string label;
	input >> label;
	labels.push_back(label);
	//cout << "added label " << label << "\n";

	readObject(label);
	
      }
      input.close();
      
  }
  
  void BOLDEvaluator::readObject(string label){
      string dir = mainDatasetDirectory +label + "/";
      string itemdir;
      
      std::ifstream input((dir+"info.txt").c_str(), std::ios::in);
      if(!input){
	cout << "error reading items..\n";
	if(input.eof()) cout << "end of file..";
	if(!input.is_open()) cout << "file not open..\n";
	input.close();
	cout << "error at label " <<label << "\n";
      }
      
      int nItems;
      input >> nItems;
      input.close();
      cout << "scanning " << nItems << " items\n";
      for(int i=0; i< nItems ; i++){
	std::stringstream fileName;
	fileName << dir <<  label << i << ".jpg";
	//cout << "reading " << fileName.str() << "\n";
	BOLDDatum d(fileName.str(),label);
	data.push_back(d);
      }
      
  }
  
  void BOLDEvaluator::splitData(float fracTest){
      int total = data.size();
      int nTest = fracTest*total;
      cout << nTest << "test data\n";
      cout << total - nTest << "trainingsdata\n";
      vector<int> testIndices(nTest);
      int randIndex;
      
      //init random unique indices
      for(int i=0;i<nTest;i++){
	 bool isPresent = true;
	 while(isPresent){
	    isPresent = false;
	    randIndex = rand() % data.size();
	    for(int j=0 ; j< testIndices.size();j++){
	      if(randIndex == testIndices[j]){
		isPresent = true;
		break;
	      }
	    }
	    //cout << "randIndex: " << randIndex << "\n";
	 }
	 testIndices.push_back(randIndex);
      }
      
      for(int i=0;i<total;i++){
	bool isTest = false;
	for(int j=0;j<testIndices.size();j++){
	  if(testIndices[j]==i) isTest = true;
	}
	
	if(isTest)
	  testSet.push_back(data[i]);
	else
	  trainingSet.push_back(data[i]);
      }
      
  }
  
  void BOLDEvaluator::train(int curFold,int totFold){
    for(int i=0;i<trainingSet.size();i++){
      if(!(totFold==0&&curFold==0))cout << "training " << trainingSet[i].label << " "<<i << " at fold " << curFold+1 << " from "<<totFold <<"\n";
      bold.addLabeledFeatureFromFile(trainingSet[i]);
      sift.train(trainingSet[i]);
    }
  }
  
  void BOLDEvaluator::test(){
    nSIFTCorrect=0;
    nSIFTFalse=0;
    nBOLDCorrect=0;
    nBOLDFalse=0;
    BOLDDatum result;
    for(int i=0;i<testSet.size();i++){
      
      
      //test sift
      result = sift.classify(testSet[i],true);
      cout << "determined label = " << result.label << "\n";
      if(result.label==testSet[i].label){
	cout << "correct!\n";
	report.updateSIFT(testSet[i],true,result);
	nSIFTCorrect++;
      }
      else{
	cout << "Wrong!\n";
	report.updateSIFT(testSet[i],false,result);
	nSIFTFalse++;
      }
      //test bold
      result=bold.classify(testSet[i]);
      cout << "determined label = " << result.label << "\n";
      if(result.label==testSet[i].label){
	cout << "correct!\n";
	report.updateBOLD(testSet[i],true,result);
	nBOLDCorrect++;
      }
      else{
	cout << "Wrong!\n";
	report.updateBOLD(testSet[i],false,result);
	nBOLDFalse++;
      }
    }
    
    
  }
  
  void BOLDEvaluator::nTests(int nFold,float fracTest,int nItems){
    int totalCorrectBOLD=0;
    int totalFalseBOLD = 0;
    int totalCorrectSIFT = 0;
    int totalFalseSIFT = 0;
    int total;
    
    std::istringstream istream;
    for(int i=0;i<nFold;i++){
      
      readDataset("BVD_M01/",nItems);
      splitData(fracTest);
      train(i,nFold);
      test();
      totalCorrectBOLD+=nBOLDCorrect;
      totalCorrectSIFT+=nSIFTCorrect;
      totalFalseBOLD+=nBOLDFalse;
      totalFalseSIFT+=nSIFTFalse;
      bold.clear();
      sift.clear();
      data.clear();
      testSet.clear();
      trainingSet.clear();
      labels.clear();
    }
    total = totalCorrectBOLD+totalFalseBOLD;
    float averageCorrectBOLD = totalCorrectBOLD*100/total;
    float averageCorrectSIFT = totalCorrectSIFT*100/total;
    float averageFalseBOLD = totalFalseBOLD*100/total;
    float averageFalseSIFT = totalFalseSIFT*100/total;
    
    cout << "BOLD: On average " << averageCorrectBOLD << "\% was correct and " << averageFalseBOLD << "\% was false\n";
    cout << "SIFT: On average " << averageCorrectSIFT << "\% was correct and " << averageFalseSIFT << "\% was false\n";
    report.writeHTML("NewestReport");
  }
  
}

int main(int argc,char**argv){
  
  cout << "Marc and Marc proudly present......\n\nBOLD\n\n";
  BOLD::BOLDEvaluator eval;

 /* if(string(argv[1]).compare("resize") == 0)
  {
    cout << "RESIZE THAT SHIIIIT\n";
    BOLD::MatBlur mattie(imread("cat.jpg", CV_LOAD_IMAGE_COLOR));
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window",     mattie.resize(2));                   // Show our image inside it.
    waitKey(0);  
  }
  else */
 
 
if(argc == 4 && string("train").compare(argv[1])==0){
  // eval.nTests(20,0.09f);
    float frac;
    int nItems;
    istringstream ss(argv[3]);
    ss >> frac;
    istringstream ass(argv[2]);
    ass >> nItems;
    eval.readDataset("BVD_M01/",nItems);
    eval.splitData(frac);
    eval.train(0,0);
    eval.bold.writeToFile("DEMO.ft");
  }else if(argc==2 && string("dia").compare(argv[1])==0) 
    eval.bold.dialogue();
  else if(argc == 5 && string("crossfold").compare(argv[1])==0){

    int nFold;
    float frac;
    int nItems;
  
    
    istringstream ss(argv[2]);
    ss >> nFold;
    istringstream ass(argv[3]);
    ass >> nItems;
    istringstream bss(argv[4]);
    bss >> frac;
    cout << "starting " << nFold << " fold crossvalidation with "<< (nItems==0? "all":"");
    if(nItems>0)cout << nItems;
    cout << " items and with " << frac*100 << "\% as testset\nPress any key to continue or ctrl-c to abort\n";
    getchar();
    eval.nTests(nFold,frac,nItems);
  }else{
    cout << "Invalid run parameter..\nType 'dia' for a dialogue\nType 'train <int:first N items of BVD_M01(0 = all items)><float:fracTestset>' to train from BVD_M01\nType 'crossfold <int:nFold> <int:first N items of dataset(0 = all items)> <float:fracTestset>' to use crossvalidation\n";
    return 0;   
  }

  return 0; 
}
