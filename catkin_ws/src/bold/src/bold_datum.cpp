#include "bold_datum.hpp"

using namespace std;

namespace BOLD{
  
 BOLDDatum::BOLDDatum(string fileDir,string lab){
  filename = fileDir;
  label =lab;
  hasDir = true;
 }
 
 BOLDDatum::BOLDDatum(string lab){
  filename = "";
  label = lab;
  hasDir = false;
 }
 
 BOLDDatum::BOLDDatum(){
  filename = "";
  label = "";
  hasDir = false;
 }
 
}