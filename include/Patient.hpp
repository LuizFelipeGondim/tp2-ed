#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <iostream>
#include <iomanip>
#include <string>

struct Date {
  double hour;
  int day;
  int month;
  int year;
};

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

  Date triageTime;
  Date patientCareTime;
  Date hospitalMeasuresTime;
  Date laboratoryTestsTime;
  Date imagingTestsTime;
  Date medicalSuppliesTime;

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

  void print();
  std::string getMonthName(int month);
  std::string getWeekdayName(int year, int month, int day);
  std::string formatHour(double hour);
};

#endif
