#include "bold_evaluator.hpp"

#include <sstream>
using namespace std;

namespace BOLD{
  
  BOLDDatum::BOLDDatum(string filedir,string lab){
   label=lab;
   filename = filedir;
  }
  
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
  
  void BOLDEvaluator::train(){
    for(int i=0;i<trainingSet.size();i++){
      bold.addLabeledFeatureFromFile(trainingSet[i].filename,trainingSet[i].label);
    }
  }
  
  void BOLDEvaluator::test(){
    nCorrect=0;
    nFalse=0;
    string result;
    for(int i=0;i<testSet.size();i++){
      result=bold.classify(testSet[i].filename);
      if(result==testSet[i].label)
	nCorrect++;
      else
	nFalse++;
    }
    
    
  }
  
  void BOLDEvaluator::nTests(int n,float fracTest){
    int totalCorrect=0;
    int totalFalse = 0;
    int total;
    for(int i=0;i<n;i++){
      
      readDataset("BVD_M01/",0);
      splitData(fracTest);
      train();
      test();
      totalCorrect+=nCorrect;
      totalFalse+=nFalse;
      bold.clear();
      data.clear();
      testSet.clear();
      trainingSet.clear();
      labels.clear();
    }
    total = totalCorrect+totalFalse;
    float averageCorrect = totalCorrect*100/total;
    float averageFalse = totalFalse*100/total;
    
    cout << "On average " << averageCorrect << "\% was correct and " << averageFalse << "\% was false\n";
  }
  
}

int main(int argc,char**argv){
  
  cout << "Marc and Marc proudly present......\n\n\n\n\n\nBOLD\n\n";
  BOLD::BOLDEvaluator eval;

  
  if(argc>1){
   // eval.nTests(20,0.09f);
    eval.readDataset("BVD_M01/",0);
    eval.splitData(0.2f);
    eval.train();
    eval.bold.writeToFile("DEMO.ft");
  }else
    eval.bold.dialogue();
  return 0; 
}