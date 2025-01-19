#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <iostream>
#include <fstream>
#include "Queue.hpp"
#include "Events.hpp"
#include "Vector.hpp"
#include "Patient.hpp"
#include "Procedure.hpp"

struct Clock{
  double hour;
  int day;
};

class Schedule {
private:
  //Verificar se ta sendo usado alem do construtor
  int numberPatients = 0;

  Clock clock;
  Events events;
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
  Queue<Patient*> serviceModerateQueue;
  Queue<Patient*> serviceSevereQueue;

  void HandleTriageQueue(Clock clock);
  void HandlePatientCare(Clock clock);
  void HandleHospitalMeasures(Clock clock);
  void HandleLaboratoryTests(Clock clock);
  void HandleImagingTests(Clock clock);
  void HandleMedicalSupplies(Clock clock);
  
public:

  Schedule(std::string fileName);
  ~Schedule();

  void StartSimulation();
  void Print();
};

#endif
