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
  double hour;
  int urgency;
  int hospitalMeasures;
  int laboratoryTests;
  int imagingTests;
  int medicalSupplies;

  double triageTime;
  double patientCareTime;
  double hospitalMeasuresTime;
  double laboratoryTestsTime;
  double imagingTestsTime;
  double medicalSuppliesTime;

  double waitingTime;
  double serviceTime;
  
  int state;

  Patient(
    int id = 0,
    int discharged = 0,
    int year = 0,
    int month = 0,
    int day = 0,
    double hour = 0.0,
    int urgency = 0,
    int hospitalMeasures = 0,
    int laboratoryTests = 0,
    int imagingTests = 0,
    int medicalSupplies = 0
  );

  void Print() const;
};

#endif
