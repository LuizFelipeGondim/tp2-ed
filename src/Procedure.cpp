#include "Procedure.hpp"

Procedure::Procedure(double averageTime, int numberUnits) : 
  averageTime(averageTime),
  numberUnits(numberUnits) { 
    units.resize(numberUnits);
  }

// Verifica se tem espaço no procedimento
int Procedure::HasSpace() {
  if (numberUnits - units.getSize() > 0)
    return numberUnits - units.getSize();

  return 0;
}