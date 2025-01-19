#ifndef PROCEDURE_HPP
#define PROCEDURE_HPP

#include <iostream>
#include "Patient.hpp"
#include "Vector.hpp"

class Procedure {
private:
  
public:
  Vector<Patient*> units;
  double averageTime;
  int numberUnits;

  Procedure(double averageTime, int numberUnits);

  int HasSpace();

};

#endif
