#ifndef PROCEDURE_HPP
#define PROCEDURE_HPP

#include <iostream>
#include "Patient.hpp"
#include "Vector.hpp"

class Procedure {
private:
  
public:
  Vector<Patient*> units;
  float averageTime;
  int numberUnits;

  Procedure(float averageTime, int numberUnits);
  ~Procedure();

  int HasSpace();

};

#endif
