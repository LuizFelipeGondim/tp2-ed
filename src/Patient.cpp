#include "Patient.hpp"

Patient::Patient(
  int id,
  int discharged,
  int year,
  int month,
  int day,
  double hour,
  int urgency,
  int hospitalMeasures,
  int laboratoryTests,
  int imagingTests,
  int medicalSupplies
) : id(id),
    discharged(discharged),
    year(year),
    month(month),
    day(day),
    hour(hour),
    urgency(urgency),
    hospitalMeasures(hospitalMeasures),
    laboratoryTests(laboratoryTests),
    imagingTests(imagingTests),
    medicalSupplies(medicalSupplies),
    triageTime(0.0),
    patientCareTime(0.0),
    hospitalMeasuresTime(0.0),
    laboratoryTestsTime(0.0),
    imagingTestsTime(0.0),
    medicalSuppliesTime(0.0),
    waitingTime(0.0),
    serviceTime(0.0),
    state(1) {}

void Patient::Print() const {
  std::cout << id
            << " " << discharged
            << " " << year << "-" << month << "-" << day
            << " " << hour
            << " " << urgency
            << " " << hospitalMeasures
            << " " << laboratoryTests
            << " " << imagingTests
            << " " << medicalSupplies
            //<< " " << hospitalMeasuresTime
            //<< " " << laboratoryTestsTime
            //<< " " << imagingTestsTime
            //<< " " << medicalSuppliesTime
            << "        " << waitingTime
            << " " << serviceTime
            << " " << state
            << std::endl;
}