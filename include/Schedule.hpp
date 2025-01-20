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
  int month;
  int year;
};

class Schedule {
private:
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

  void handleTriageQueue(Clock clock);
  void handlePatientCare(Clock clock);
  void handleHospitalMeasures(Clock clock);
  void handleLaboratoryTests(Clock clock);
  void handleImagingTests(Clock clock);
  void handleMedicalSupplies(Clock clock);

  bool isLeapYear(int year);
  void insertEvent(int id, Clock clock, double serviceTime);
  double convertToTotalHours(Clock date);
  double convertToTotalHours(Date date);
  void startSimulation();

public:

  Schedule(std::string fileName);
  ~Schedule();

  void print();
};

#endif
