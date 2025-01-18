#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <iostream>
#include <fstream>
#include "Queue.hpp"
#include "Vector.hpp"
#include "Patient.hpp"
#include "Procedure.hpp"

struct Clock{
  double hour;
  int day;
};

class Schedule {
private:
  Clock clock;
  int numberPatients = 0;
  Vector<Procedure*> procedures;
  Vector<Patient*> patients;

  //Fila de triagem
  Queue<Patient*> triageQueue;

  //Filas de atendimento
  Queue<Patient*> patientCareMildQueue;
  Queue<Patient*> patientCareModerateQueue;
  Queue<Patient*> patientCareSevereQueue;

  //Filas de pós atendimento
  Queue<Patient*> serviceMildQueue;
  Queue<Patient*> serviceCModerateQueue;
  Queue<Patient*> serviceSevereQueue;
  
public:

  Schedule(std::string fileName);
  ~Schedule();

  void StartSimulation();
  void Print();
};

#endif
