#include "bold_recognizer.hpp"
#include <fstream>

using namespace std;
using namespace cv;
using namespace BOLD;

namespace BOLD{
  
  BOLDRecognizer::BOLDRecognizer(){

  }
  //TODO: KNN adds each line combo double. Hists should be devided by two
  
  string BOLDRecognizer::classify(BOLDFeature f)
 {
    
    int nTrainedElements = trainedFeatures.size();
    int kNearestNeighborIndices[K_NEAREST_NEIGHBORS];
    double distances[K_NEAREST_NEIGHBORS];
    
    double dist;
    double buffer;
    int buff2, buff3;
    
    int labelFrequencies[K_NEAREST_NEIGHBORS];
    int highestFrequency = 0;
    int frequencyOccurences = 0;
    
    cout << "start knn..\n";
    
    for(int i = 0; i < K_NEAREST_NEIGHBORS; ++i)
    { 
      distances[i] = DBL_MAX;
      kNearestNeighborIndices[i] = -1;
      labelFrequencies[i] = 0;
    }
    
    for(int i = 0; i < nTrainedElements; ++i)
    {  
      dist = f.distanceFrom(trainedFeatures.at(i));  //store distance to compared element
      
      buff2 = i;
      for(int j = 0; j < K_NEAREST_NEIGHBORS; ++j)
       {
        if(dist <= distances[j])  
         {
            for(int n = j; n < K_NEAREST_NEIGHBORS; ++n)
             {

	            //store dist to be switched in buffer
	            buffer = distances[n];  
	            //store new dist in distances
	            distances[n] = dist;
	            //set dist for replacement at next iteration
	            dist = buffer;
	    
	            buff3 = kNearestNeighborIndices[n];
	            kNearestNeighborIndices[n] = buff2;
	            buff2 = buff3;
		    
		  
	      }
	      //for(int k=0;k<K_NEAREST_NEIGHBORS;k++)
		//cout << "ind: " << kNearestNeighborIndices[k] << "\n";
	  }
        }
     }
 
    
    //count label frequencies
    
    for(int i = 0; i < K_NEAREST_NEIGHBORS; ++i) 
    {
        for(int j = 0;j <= i; ++j) 
            if(kNearestNeighborIndices[i] != -1 && kNearestNeighborIndices[j] != -1 
            && trainedFeatures[kNearestNeighborIndices[i]].getLabel() == 
            trainedFeatures[kNearestNeighborIndices[j]].getLabel())
            {
	            ++labelFrequencies[j]; 
	            if(labelFrequencies[j] > highestFrequency)
	                highestFrequency = labelFrequencies[j];
            }
    }
    //check for ties and return label
    buff2 = -1;
    
    for(int i = 0; i < K_NEAREST_NEIGHBORS; ++i){  // debug and knn info
      if(kNearestNeighborIndices[i]<0||kNearestNeighborIndices[i]>trainedFeatures.size()){
	cout << "KNN index out of bounds!!!  : "<< kNearestNeighborIndices[i] <<"\n";
	continue;
      }
      cout << "KNN:" << i << "dist = " << distances[i] << "\tarrayIndex = " << kNearestNeighborIndices[i] <<" label = "+trainedFeatures[kNearestNeighborIndices[i]].getLabel()+ "\n";
    }
    for(int i = 0; i < K_NEAREST_NEIGHBORS; ++i)
    {
        if(labelFrequencies[i] == highestFrequency) 
        {
            return trainedFeatures[kNearestNeighborIndices[i]].getLabel();
	           /* if(buff2 == -1)
		    {
	                buff2 = i;
	            } else 
	               cout << "K-NN TIE!! Needs better tiebreaker... Choosing at random\n" ;
		       
                   cout << "KNN labels:\n";
		    
	               for(int p = 0; p < K_NEAREST_NEIGHBORS; ++p)
                   {
	                if(kNearestNeighborIndices[p] != -1)
	                    cout << trainedFeatures[kNearestNeighborIndices[p]].getLabel() 
                             << ": dist = " << distances[p] << "\n";
	                else{
	                    cout << kNearestNeighborIndices[p] << "\n" ;
	                }
	                return trainedFeatures[i].getLabel();
	            }*/
	  
        }
    }
    return trainedFeatures[buff2].getLabel();
    }
    
  
  string BOLDRecognizer::classify(string fileName){
    string label;
    descriptor.setImage(fileName,false);
    descriptor.describe();
    label = classify(descriptor.getFeature());
    descriptor.clear();
    return label;
  }
  
  void BOLDRecognizer::addLabeledFeature(BOLDFeature f){
   if(f.getLabel()=="") {
    cout << "BOLD::BOLDRecognizer::addLabeledFeature(): WARNING! Trying to add labeled feature, but feature label is not set.. Feature not added.\n" ;
    return;
   }
   if(!f.isNormalized()){
    cout << "BOLD::BOLDRecognizer::addLabeledFeature(): WARNING! Trying to add labeled feature, but feature is not yet normalized.. Feature not added.\n" ;
    return;
   }
   
    trainedFeatures.push_back(f);
  }
  
  
  void BOLDRecognizer::addLabeledFeatureFromFile(string fileName,string label){
    descriptor.setImage(fileName,false);
    descriptor.describe();
    descriptor.setFeatureLabel(label);
    cout << "push it\n";
    BOLDFeature f = descriptor.getFeature();
    trainedFeatures.push_back(f);
    descriptor.clear();
    cout << "labeled feature " + label + " from " + fileName + " has been added to the trainingset\n";
  }
  
  void BOLDRecognizer::writeToFile()
  {
      std::ofstream output("test.marc",std::ofstream::out);
      output << trainedFeatures.size() << '\n';
      output << HISTOGRAM_SIZE << '\n';

      for(size_t idx = 0; idx != trainedFeatures.size(); ++idx)
      {
        trainedFeatures[idx].writeTo(output);
      }
      
      output.close();
  }

 void BOLDRecognizer::readFromFile()
 {
        std::ifstream input("test.marc", std::ifstream::in);
        int histosize;
        int plaatjesn;
	cout << "reading..\n";
        input >> plaatjesn;
        
        input >> histosize;
	
        if(histosize != HISTOGRAM_SIZE){
            cout << "Histogram size "<<histosize <<" from file does not match the HISTOGRAM_SIZE as compiled with..\n";
	    input.close();
	    return;
	}
        for(size_t idx = 0 ; idx != plaatjesn; ++idx)
        {
	  
            double histogram[HISTOGRAM_SIZE][HISTOGRAM_SIZE];//  [alphas][betas]
            int entries;
            bool normalized;
            string label;

            for(size_t r = 0; r < HISTOGRAM_SIZE; ++r)
            {
                for(size_t k = 0; k < HISTOGRAM_SIZE; ++k)
                {
                    input >> histogram[r][k];
                }
            }

            input >> entries;
            input >> normalized;
            input >> label;

            trainedFeatures.push_back(BOLDFeature(histogram,entries,normalized,label));
            cout << idx << "\n";;
        }
        
        input.close();
 }
 
  
  void BOLDRecognizer::dialogue(){
    char i;
    string name;
    string label;
    
    cout << "Welcome to the BOLD recognizer!\n" ;
    cout << "Trainings phase:\n";
    for(;;){
      cout << "Do you want to add a trainings sample? (y=yes, n=no)\n";
      cin >> i;
      if(i=='y'){
	
	    cout << "Please specify the input file..\n";
	    cin >> name;
	    cout << "Please specify the label..\n";
	    cin >> label;
	    addLabeledFeatureFromFile(name,label);
	
        }else if(i == 'n'){
	        cout << "Ending training input..\n";
	        break;
        }else{
	        cout << "Invalid input.. try again!\n";
        }
        cout << "There are " << trainedFeatures.size() << " trained features present.\n";
    }
    //writeToFile();
    cout << "Testing phase:\n";
    for(;;){
      cout << "Do you want to present a test sample?(y=yes,n=no)\n";
      cin >> i;
      if(i=='y'){
	    cout << "Please specify the input file..\n";
	    cin >> name;
	    cout << "Determined label = " + classify(name) + "\n";
        }else if(i=='n'){
	        cout << "Goodbye..\n";
	        return;
        }else{
	        cout << "Invalid input! Try again..\n";
        }
    }
  }
  
}

int main(int argc, char**argv){
  ros::init(argc, argv, "bold");
  cv::initModule_nonfree();
  cout << argc << '\n';
  
  BOLDRecognizer br;
  br.readFromFile();
  br.dialogue();
  
  /*BOLDescriptor d;
  for(int i=1;i<argc;i++){
    cout << argv[i] << "\n";
    d.setImage(argv[i],false);
    d.setImageName(argv[i]);
    d.describe();
    d.showLines();
    d.showFeatures();
    d.clear();
    
  }*/
  
  //waitKey(0);
  ros::shutdown();
  return 0;
}
