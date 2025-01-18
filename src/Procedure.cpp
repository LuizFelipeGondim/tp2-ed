#include "Procedure.hpp"

Procedure::Procedure(float averageTime, int numberUnits) : 
  averageTime(averageTime),
  numberUnits(numberUnits) { 
    units.resize(numberUnits);
  }

Procedure::~Procedure() {
  int unitsSize = units.getSize();
  for (int i = 0; i < unitsSize; i++)
    delete units[i];
}

// Verifica se tem espaço no procedimento
int Procedure::HasSpace() {
  if (numberUnits - units.getSize() > 0)
    return numberUnits - units.getSize();

  return 0;
}