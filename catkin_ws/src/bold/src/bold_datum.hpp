#ifndef BOLD_DATUM_HPP
#define BOLD_DATUM_HPP
#include <iostream>
#include <string>
using namespace std;

namespace BOLD{
  class BOLDDatum{
      public:
      string filename,label;
      bool hasDir;
      BOLDDatum(string filedir,string lab);
      BOLDDatum(string lab);
      BOLDDatum();
    };
}

#endif