#include "Schedule.hpp"

Schedule::Schedule(std::string fileName) {
  std::ifstream file(fileName);

  if (!file.is_open()) {
    throw std::runtime_error("Erro: Arquivo não encontrado.");
  }

  float averageTime; 
  int numberUnits;

  for (int i = 0; i < 6; i++) {
    file >> averageTime >> numberUnits;

    procedures.push(new Procedure(averageTime, numberUnits));
  }

  file >> numberPatients; 

  for (int i = 0; i < numberPatients; ++i) {
    int id, discharged, year, month, day;
    float hour; 
    int urgency, hospitalMeasures, laboratoryTests, imagingTests, medicalSupplies;

    file >> id >> discharged >> year >> month >> day >> hour >> urgency
         >> hospitalMeasures >> laboratoryTests >> imagingTests >> medicalSupplies;

    patients.push(new Patient(
      id, discharged, year, month, day, hour, urgency,
      hospitalMeasures, laboratoryTests, imagingTests, medicalSupplies
    ));
  }

  //Setar clock para horário de chegada do primeiro paciente
  clock.day = patients[0]->day;
  clock.hour = patients[0]->hour;

  file.close();
}

Schedule::~Schedule() {
  int proceduresSize = procedures.getSize();
  for (int i = 0; i < proceduresSize; i++)
    delete procedures[i];

  for (int i = 0; i < numberPatients; i++)
    delete patients[i];
}

void Schedule::StartSimulation() {
  bool finishSimulation = false;

  if (patients.getSize() == 0)
    finishSimulation = true;
  
  while(finishSimulation) {

    for (int i = 0; i < patients.getSize(); i++) {
      if (patients[i]->day == clock.day && patients[i]->hour == clock.hour) {
        patients[i]->state++;
        triageQueue.Enqueue(patients[i]);
      }
    }

    // Verifica se tem espaço no serviço de trigaem, se tiver, envia o primeiro da fila. 
    // Caso não tenha espaço e a fila de triagem ainda tenha pessoas, aumenta o tempo de espera de cada uma.
    int hasSpace = procedures[0]->HasSpace();
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        if (!triageQueue.Empty()) {
          Patient* patient = triageQueue.Dequeue();
          patient->triageTime += procedures[0]->averageTime;
          patient->serviceTime += procedures[0]->averageTime;
          patient->state++;
          // Depois fazer ajuste pra quando for de um dia pro outro
          patient->nextServiceTime = clock.hour + procedures[0]->averageTime;
          procedures[0]->units.push(patient);
        }
      }
    } else if (!triageQueue.Empty()) {
      for (int i = 0; i < triageQueue.GetSize(); i++){
        Patient* patient = triageQueue.Dequeue();
        patient->waitingTime += 0.05;
        triageQueue.Enqueue(patient);
      }
    }
  }
}

void Schedule::Print() {

  for (int i = 0; i < numberPatients; i++) 
    patients[i]->Print();
  
  //std::cout << clock.day << " " << clock.hour << std::endl;
}