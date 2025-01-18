#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <iostream>

class Patient {
private:

public:
  int id;
  int discharged;
  int year;
  int month;
  int day;
  float hour;
  int urgency;
  int hospitalMeasures;
  int laboratoryTests;
  int imagingTests;
  int medicalSupplies;

  float nextServiceTime;
  float triageTime;
  float patientCareTime;
  float hospitalMeasuresTime;
  float laboratoryTestsTime;
  float imagingTestsTime;
  float medicalSuppliesTime;

  float waitingTime;
  float serviceTime;
  
  int state;

  Patient(
    int id = 0,
    int discharged = 0,
    int year = 0,
    int month = 0,
    int day = 0,
    float hour = 0,
    int urgency = 0,
    int hospitalMeasures = 0,
    int laboratoryTests = 0,
    int imagingTests = 0,
    int medicalSupplies = 0
  );

  void Print() const;
};

#endif
