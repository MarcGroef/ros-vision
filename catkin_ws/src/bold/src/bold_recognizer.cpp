#include "bold_recognizer.hpp"


using namespace std;
using namespace cv;
using namespace BOLD;

namespace BOLD{
  
  BOLDRecognizer::BOLDRecognizer(){

  }
  


  
  BOLDDatum BOLDRecognizer::classify(BOLDFeature* f)
 {
   
    int nTrainedElements = trainedFeatures.size();


    
    double dist;
    double buffer;
    int buff2, buff3;
    

    int highestFrequency = 0;
    int frequencyOccurences = 0;
    
    int KNlabelFrequencies[K_NEAREST_NEIGHBORS];

    //cout << "start knn..\n";
    
    for(int i = 0; i < K_NEAREST_NEIGHBORS_MEMORY_SIZE; ++i)
    { 
      distances[i] = DBL_MAX;
      kNearestNeighborIndices[i] = -1;
      labelFrequencies[i] = 0;
    }
    
    for(int i=0;i < K_NEAREST_NEIGHBORS;i++)
      KNlabelFrequencies[i]=0;
    
    
    for(int i = 0; i < nTrainedElements; ++i)
    {  
      dist = f->manhatDistanceFrom(trainedFeatures[i]);  //store distance to compared element
  
      buff2 = i;
      for(int j = 0; j < K_NEAREST_NEIGHBORS_MEMORY_SIZE; ++j)
       {
        if(dist <= distances[j])  
         {
            for(int n = j; n < K_NEAREST_NEIGHBORS_MEMORY_SIZE; ++n)
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
    //cout << "count freqs\n";
    for(int i = 0; i < K_NEAREST_NEIGHBORS_MEMORY_SIZE; ++i) 
    {
	cout << "*******************knn list*****************\n"  << distances[i] << "\n";
        for(int j = 0;j <= i; ++j) 
	{
	    //cout << "i " << i << " " << trainedFeatures[kNearestNeighborIndices[i]]->getLabel() << " j " << j << " fr " << labelFrequencies[j] << "\n"; 
	    
            if(kNearestNeighborIndices[i] != -1 && kNearestNeighborIndices[j] != -1 
            && trainedFeatures[kNearestNeighborIndices[i]]->getLabel() ==
            trainedFeatures[kNearestNeighborIndices[j]]->getLabel())
            {	
		    if(i < K_NEAREST_NEIGHBORS)
		      ++KNlabelFrequencies[j];
		    
		    ++labelFrequencies[j];
	            
	            if(i < K_NEAREST_NEIGHBORS && labelFrequencies[j] > highestFrequency)  //only use K_NEAREST_NEIGHBORS as frequency count
	                highestFrequency = KNlabelFrequencies[j];
		   
		    break;
            }
	}
    }
    //check for ties and return label
    buff2 = -1;
    cout << "check for ties and return label\n";
    for(int i = 0; i < K_NEAREST_NEIGHBORS; ++i){  // debug and knn info
      if(kNearestNeighborIndices[i]<0||kNearestNeighborIndices[i]>trainedFeatures.size()){
	cout << "KNN index out of bounds!!!  : "<< kNearestNeighborIndices[i] <<"\n";
	continue;
      }
      cout << "KNN:" << i << "dist = " << distances[i] << "\tarrayIndex = " << kNearestNeighborIndices[i] <<" label = "+trainedFeatures[kNearestNeighborIndices[i]]->getLabel()+ "\n";
    }
    for(int i = 0; i < K_NEAREST_NEIGHBORS; ++i)
    {
	cout << "freq neigboor " << i << " : " << labelFrequencies[i] << "\n";
        if(KNlabelFrequencies[i] == highestFrequency) 
        {

            return trainedFeatures[kNearestNeighborIndices[i]]->getDatum();
        }
    }
   
    return BOLDDatum();
  }
  
  bool larger(int i,int j){
   return (i>j) ;
  }
  
  vector<BOLDDatum> BOLDRecognizer::getSortedLabels(){
    vector<int> freqs(K_NEAREST_NEIGHBORS_MEMORY_SIZE); 
    int frequencies[K_NEAREST_NEIGHBORS_MEMORY_SIZE];
    
    std::vector<BOLDDatum> sortedLabels;
   
    for(int i=0;i<K_NEAREST_NEIGHBORS_MEMORY_SIZE;i++) {
      freqs.push_back(labelFrequencies[i]);
      frequencies[i]=labelFrequencies[i];
    }
    //cout << "sort\n";
    std::sort(freqs.begin(),freqs.end(),larger);
   // cout << "sorted\n";
    int matches=0;
    for(int i=0;i<K_NEAREST_NEIGHBORS_MEMORY_SIZE && freqs[matches]!=0 ;i++){
      if(frequencies[i]==freqs[matches]){
	sortedLabels.push_back(trainedFeatures[kNearestNeighborIndices[i]]->getDatum());
	cout << "pushed  " << trainedFeatures[kNearestNeighborIndices[i]]->getDatum().filename << "\n";
	frequencies[i]=0; // mark it
	matches++;
	i=0;
      }
    }
    return sortedLabels;
  }
  
  BOLDDatum BOLDRecognizer::classify(BOLDDatum datum){
    cout << "classify " << datum.filename << "\n";
    BOLDDatum label;
    std::clock_t start;
    start = std::clock();
    descriptor.setImage(datum.filename,false);
    descriptor.describe();
    label = classify(descriptor.getFeature());
    descriptor.clear();
    descriptor.freeFeature();
    cout << "It took "<<(float)(std::clock()-start)/CLOCKS_PER_SEC <<" sec to process : This would result in "<< 1/((float)(std::clock()-start)/CLOCKS_PER_SEC)<< " fps\n";
    return label;
  }
  
  void BOLDRecognizer::addLabeledFeature(BOLDFeature* f){
   if(f->getLabel()=="") {
    cout << "BOLD::BOLDRecognizer::addLabeledFeature(): WARNING! Trying to add labeled feature, but feature label is not set.. Feature not added.\n" ;
    return;
   }
   if(!f->isNormalized()){
    cout << "BOLD::BOLDRecognizer::addLabeledFeature(): WARNING! Trying to add labeled feature, but feature is not yet normalized.. Feature not added.\n" ;
    return;
   }
   
    trainedFeatures.push_back(f);
  }
  
  
  void BOLDRecognizer::addLabeledFeatureFromFile(BOLDDatum datum){
    descriptor.setImage(datum.filename,false);
    descriptor.describe();
    descriptor.setFeatureLabel(datum);
   // cout << "push it\n";
    BOLDFeature* f = descriptor.getFeature();
    trainedFeatures.push_back(f);
    descriptor.clear();
   // cout << "labeled feature " + label + " from " + fileName + " has been added to the trainingset\n";
  }
  
  void BOLDRecognizer::addLabeledFeature(Mat &image,std::string label){
    BOLDDatum dat(label);
    descriptor.setImage(image,false);
    descriptor.describe();
    descriptor.setFeatureLabel(dat);
    trainedFeatures.push_back(descriptor.getFeature());
    descriptor.clear();
  }
  
  void BOLDRecognizer::writeToFile(string fileName)
  {
      std::ofstream output(fileName.c_str(),std::ofstream::out);
      output << trainedFeatures.size() << '\n';
      output << HISTOGRAM_SIZE << '\n';

      for(size_t idx = 0; idx != trainedFeatures.size(); ++idx)
      {
        trainedFeatures[idx]->writeTo(output);
      }
      
      output.close();
  }

 void BOLDRecognizer::readFromFile(string fileName)
 {
        std::ifstream input(fileName.c_str(), std::ifstream::in);
        int histosize;
        int plaatjesn;
	cout << "reading from File..\n";
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
	    string dir;
	    //TODO: Optimize by writing directly to a new boldfeature
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
	    input >> dir;
	    BOLDDatum datum(dir,label);
            trainedFeatures.push_back(new BOLDFeature(histogram,entries,normalized,datum));
            cout << idx << " " << label << "\n";;
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
      cout << "Do you want to add a trainings sample? (y=yes, n=no, s=store trained features to file,l=load features from file)\n";
      cin >> i;
      if(i=='y'){
	
	    cout << "Please specify the input file..\n";
	    cin >> name;
	    cout << "Please specify the label..\n";
	    cin >> label;
	    BOLDDatum datum(name,label);
	    addLabeledFeatureFromFile(datum);
	
        }else if(i == 'n'){
	        cout << "Ending training input..\n";
	        break;
	}else if(i=='s'){
		writeToFile("DEMO.ft");
	}else if(i=='l'){
		readFromFile("DEMO.ft");
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
	    BOLDDatum dat(name,"");
	    cout << "Determined label = " << classify(dat).label << "\n";
        }else if(i=='n'){
	        cout << "Goodbye..\n";
	        return;
        }else{
	        cout << "Invalid input! Try again..\n";
        }
    }
  }
  void BOLDRecognizer::showAllFeatures(){
    cout << "show features\n";
    string s = "a";
    string prevLabel = "";
    for(int i=0;i<trainedFeatures.size();i++){
      if(prevLabel==trainedFeatures[i]->getLabel()){
	s[0]++;
      }else{
	prevLabel = trainedFeatures[i]->getLabel();
	s="a";
      }
      cout << trainedFeatures[i]-> getLabel();
      trainedFeatures[i]->show(trainedFeatures[i]->getLabel()+" "+s);
    }
  }

   BOLDRecognizer::~BOLDRecognizer()
   {
    for(std::vector<BOLDFeature*>::iterator it = trainedFeatures.begin(); it != trainedFeatures.end(); ++it)
        delete *it;
    
   }
   
   void BOLDRecognizer::clear(){
      descriptor.clear();
      
      for(std::vector<BOLDFeature*>::iterator it = trainedFeatures.begin(); it != trainedFeatures.end(); ++it)
        delete *it;
      trainedFeatures.clear();
   }
   
   
  
}


/*int main(int argc,char**argv){
    BOLDRecognizer br;
    br.dialogue();
    //br.showAllFeatures();  //***causes memleak indirectly lost:240, definitivly 60 bytes, posswibly lost: 228.865 bytes, still reachable: 796.541 bytes
    
    //waitKey(0);
    //cv::destroyAllWindows();
    cout << "Goodbye!\n";
    return 0;   
}

*/