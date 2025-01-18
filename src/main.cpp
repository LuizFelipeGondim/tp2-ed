#include <iostream>
#include <unistd.h>
#include "Schedule.hpp"

int main(int argc, char *argv[]) {
  std::string fileName;

  try {

    if (optind < argc) {
      fileName = argv[optind]; 
    } else {
      throw std::runtime_error("Erro: Nome do arquivo não especificado.");
    }
    Schedule schedule(fileName);
    schedule.Print();
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
  }
  
  

  /*
  Patient opa;

  for (int i = 0; i < numberOffila; ++i) {
    opa = fila.Dequeue();
    opa.print();
  }

  file.close();*/

  return 0;
}