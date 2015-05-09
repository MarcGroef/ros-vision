#ifndef BOLD_VECTOR_HPP
#define BOLD_VECTOR_HPP

#include <iostream>
#include <cstdlib>

namespace BOLD{
  
  enum VectorConstants{
    VECTOR_SIZE = 3,  //cross product assumes value 3. If used otherwise: exit(1)
  };
  
 
  
  class BVector{
  private:
    double a[3];
  public:
    
    BVector(double* array);
    BVector(double a1,double a2,double a3);
    double getElement(int index);
    double dot(BVector b);
    BVector cross(BVector c);
  };
  
  
}

#endif