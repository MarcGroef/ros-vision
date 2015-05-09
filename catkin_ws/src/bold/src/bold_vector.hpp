#ifndef BOLD_VECTOR_HPP
#define BOLD_VECTOR_HPP

#include <iostream>
#include <cstdlib>

#include <math.h>


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
    BVector();
    void set(double a1,double a2,double a3);
    void setElement(int index,double value);
    double getElement(int index);
    double dot(BVector b);
    double abs();
    double abs2D();
    BVector minus(BVector b);
    BVector plus(BVector b);
    BVector cross(BVector b);
    BVector divByScalar(double b);
    BVector timesScalar(double b);
  };
  
  
}

#endif