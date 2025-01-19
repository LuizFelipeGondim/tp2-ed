#include "Schedule.hpp"

Schedule::Schedule(std::string fileName) {
  std::ifstream file(fileName);

  if (!file.is_open()) {
    throw std::runtime_error("Erro: Arquivo não encontrado.");
  }

  double averageTime; 
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
  
  int size = patients.getSize();
  for (int i = 0; i < size; i++) {
    events.push(Event(
      patients[i]->id,
      patients[i]->hour,
      patients[i]->day,
      patients[i]->month,
      patients[i]->year
    ));
  }

  while(!finishSimulation) {
    int hasSpace;
    Event event = events.top();
    clock.hour = event.hour;

    Patient* patient;
    for (int i = 0; i < patients.getSize(); i++)
      if (event.id == patients[i]->id) 
        patient = patients[i];

    std::cout << event.id << " " << clock.hour << std::endl;

    if (patient->state == 1) {
      patient->state++;
      patient->triageTime = clock.hour; // Ajustar pra mudança de dia e mês
      triageQueue.Enqueue(patient);
    }
    
    if (patient->state == 3) {

      // Verifica se algum paciente já terminou a triagem.
      // Se terminou, ele é mandado para a fila de espera do atendimento.
      patient->state++;
      patient->patientCareTime = clock.hour; // Ajustar pra mudança de dia e mês

      if (patient->urgency == 0) {
        patientCareMildQueue.Enqueue(patient);
      } else if (patient->urgency == 1) {
        patientCareModerateQueue.Enqueue(patient);
      } else if (patient->urgency == 2) {
        patientCareSevereQueue.Enqueue(patient);
      } 

      for (int i = 0; i < procedures[0]->units.getSize(); i++)
        if (patient->id == procedures[0]->units[i]->id)
          procedures[0]->units.pop(i);
    }

    // Verifica se algum paciente terminou alguns dos serviços.
    // Se terminou, ele é encaminhado pra sua fila.
    
    if (patient->state >= 5 && patient->state <= 13) {

      // Verifica se o paciente teve alta, caso afirmativo, seu atendimento é encerrado.
      // Caso não tenha recebido alta, ele será transferido para as outras filas de prioridade.
      if (patient->state == 5 && patient->discharged) {
        if (patient->state == 5) {
          for (int i = 0; i < procedures[1]->units.getSize(); i++)
            if (patient->id == procedures[1]->units[i]->id)
              procedures[1]->units.pop(i);
        }
        patient->state = 14;
      } else {
        
        // Pula estado caso a quantidade de um serviço é zero 
        if (patient->state == 5) {
          for (int i = 0; i < procedures[1]->units.getSize(); i++)
            if (patient->id == procedures[1]->units[i]->id)
              procedures[1]->units.pop(i);
        }

        if (patient->state == 5 && patient->hospitalMeasures == 0)
          patient->state = 7;

        if (patient->state == 7) {
          for (int i = 0; i < procedures[2]->units.getSize(); i++)
            if (patient->id == procedures[2]->units[i]->id)
              procedures[2]->units.pop(i);
        }

        if (patient->state == 7 && patient->laboratoryTests == 0)
          patient->state = 9;
        
        if (patient->state == 9) {
        for (int i = 0; i < procedures[3]->units.getSize(); i++)
          if (patient->id == procedures[3]->units[i]->id)
            procedures[3]->units.pop(i);
        }

        if (patient->state == 9 && patient->imagingTests == 0)
          patient->state = 11;
        
        if (patient->state == 11) {
          for (int i = 0; i < procedures[4]->units.getSize(); i++)
            if (patient->id == procedures[4]->units[i]->id)
              procedures[4]->units.pop(i);
        }

        if (patient->state == 11 && patient->medicalSupplies == 0)
          patient->state = 13;

        if (patient->state == 13)
        for (int i = 0; i < procedures[5]->units.getSize(); i++)
          if (patient->id == procedures[5]->units[i]->id)
            procedures[5]->units.pop(i);

        patient->state++;

        // Encaminha pacientes que ainda precisam ir ser atendidos para as filas
        if (patient->state < 13) {
          if (patient->urgency == 0) {
            serviceMildQueue.Enqueue(patient);
          } else if (patient->urgency == 1) {
            serviceModerateQueue.Enqueue(patient);
          } else if (patient->urgency == 2) {
            serviceSevereQueue.Enqueue(patient);
          }
        }

        if (patient->state == 6) 
          patient->hospitalMeasuresTime = clock.hour; // Ajustar pra mudança de dia e mês

        if (patient->state == 8) 
          patient->laboratoryTestsTime = clock.hour; // Ajustar pra mudança de dia e mês
        
        if (patient->state == 10) 
          patient->imagingTestsTime = clock.hour; // Ajustar pra mudança de dia e mês

        if (patient->state == 12) 
          patient->medicalSuppliesTime = clock.hour; // Ajustar pra mudança de dia e mês
      }
    }

    // Verifica se tem espaço no serviço de triagem, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[0]->HasSpace();
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        if (!triageQueue.Empty()) {
          HandleTriageQueue(clock);
        }
      }
    }

    hasSpace = procedures[1]->HasSpace();
    std::cout << hasSpace << std::endl;
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        HandlePatientCare(clock);
      }
    }

    hasSpace = procedures[2]->HasSpace();
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        HandleHospitalMeasures(clock);
      }
    }

    hasSpace = procedures[3]->HasSpace();
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        HandleLaboratoryTests(clock);
      }
    }
    

    hasSpace = procedures[4]->HasSpace();
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        HandleImagingTests(clock);
      }
    }

    hasSpace = procedures[5]->HasSpace();
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        HandleMedicalSupplies(clock);
      }
    }

    events.pop();

    if (events.empty()) {

      if (!triageQueue.Empty()) {
        HandleTriageQueue(clock);
      } else if (
        !patientCareMildQueue.Empty() || 
        !patientCareModerateQueue.Empty() || 
        !patientCareSevereQueue.Empty()
      ) {
        HandlePatientCare(clock);
      } else if (
        !serviceMildQueue.Empty() || 
        !serviceModerateQueue.Empty() || 
        !serviceSevereQueue.Empty()
      ) {
        HandleHospitalMeasures(clock);
        HandleLaboratoryTests(clock);
        HandleImagingTests(clock);
        HandleMedicalSupplies(clock);
      } else {
        finishSimulation = true;
      }

    }
  }
}

void Schedule::HandleTriageQueue(Clock clock) {
  Patient* patientTriage = triageQueue.Dequeue();

  patientTriage->waitingTime += clock.hour - patientTriage->triageTime; // Ajustar pra mudança de dia e mês
  patientTriage->serviceTime += procedures[0]->averageTime;
  patientTriage->state++;
  procedures[0]->units.push(patientTriage);

  events.push(Event(
    patientTriage->id,
    clock.hour + procedures[0]->averageTime, // Ajustar pra mudança de dia e mês
    patientTriage->day,
    patientTriage->month,
    patientTriage->year
  ));
}

void Schedule::HandlePatientCare(Clock clock) {
  Patient* patientCare = nullptr;
  
  if (!patientCareSevereQueue.Empty()) {
    patientCare = patientCareSevereQueue.Dequeue();
  } else if (!patientCareModerateQueue.Empty()) {
    patientCare = patientCareModerateQueue.Dequeue();
  } else if (!patientCareMildQueue.Empty()) {
    patientCare = patientCareMildQueue.Dequeue();
  }

  if (patientCare != nullptr) {
    patientCare->waitingTime += clock.hour - patientCare->patientCareTime; // Ajustar pra mudança de dia e mês
    patientCare->serviceTime += procedures[1]->averageTime;
    patientCare->state++;
    procedures[1]->units.push(patientCare);

    events.push(Event(
      patientCare->id,
      clock.hour + procedures[1]->averageTime, // Ajustar pra mudança de dia e mês
      patientCare->day,
      patientCare->month,
      patientCare->year
    ));
  }
}

void Schedule::HandleHospitalMeasures(Clock clock) {
  Patient* patientService = nullptr;
        
  if (!serviceSevereQueue.Empty()) {
    patientService = serviceSevereQueue.Front();
    if (patientService->state == 6) serviceSevereQueue.Dequeue();
  } else if (!serviceModerateQueue.Empty()) {
    patientService = serviceModerateQueue.Front();
    if (patientService->state == 6) serviceModerateQueue.Dequeue();
  } else if (!serviceMildQueue.Empty()) {
    patientService = serviceMildQueue.Front();
    if (patientService->state == 6) serviceMildQueue.Dequeue();
  }

  if (patientService != nullptr && patientService->state == 6) {
    patientService->waitingTime += clock.hour - patientService->hospitalMeasuresTime; // Ajustar pra mudança de dia e mês
    patientService->serviceTime += patientService->hospitalMeasures * procedures[2]->averageTime;
    patientService->state++;
    procedures[2]->units.push(patientService);

    events.push(Event(
      patientService->id,
      clock.hour + (patientService->hospitalMeasures * procedures[2]->averageTime), // Ajustar pra mudança de dia e mês
      patientService->day,
      patientService->month,
      patientService->year
    ));
  }
} 

void Schedule::HandleLaboratoryTests(Clock clock) {
  Patient* patientService = nullptr;
  
  if (!serviceSevereQueue.Empty()) {
    patientService = serviceSevereQueue.Front();
    if (patientService->state == 8) serviceSevereQueue.Dequeue();
  } else if (!serviceModerateQueue.Empty()) {
    patientService = serviceModerateQueue.Front();
    if (patientService->state == 8) serviceModerateQueue.Dequeue();
  } else if (!serviceMildQueue.Empty()) {
    patientService = serviceMildQueue.Front();
    if (patientService->state == 8) serviceMildQueue.Dequeue();
  }

  if (patientService != nullptr && patientService->state == 8) {
    patientService->waitingTime += clock.hour - patientService->laboratoryTestsTime; // Ajustar pra mudança de dia e mês
    patientService->serviceTime += patientService->laboratoryTests * procedures[3]->averageTime;
    patientService->state++;
    procedures[3]->units.push(patientService);

    events.push(Event(
      patientService->id,
      clock.hour + (patientService->laboratoryTests * procedures[3]->averageTime), // Ajustar pra mudança de dia e mês
      patientService->day,
      patientService->month,
      patientService->year
    ));
  }
} 

void Schedule::HandleImagingTests(Clock clock) {
  Patient* patientService = nullptr;
  
  if (!serviceSevereQueue.Empty()) {
    patientService = serviceSevereQueue.Front();
    if (patientService->state == 10) serviceSevereQueue.Dequeue();
  } else if (!serviceModerateQueue.Empty()) {
    patientService = serviceModerateQueue.Front();
    if (patientService->state == 10) serviceModerateQueue.Dequeue();
  } else if (!serviceMildQueue.Empty()) {
    patientService = serviceMildQueue.Front();
    if (patientService->state == 10) serviceMildQueue.Dequeue();
  }

  if (patientService != nullptr && patientService->state == 10) {
    patientService->waitingTime += clock.hour - patientService->imagingTestsTime; // Ajustar pra mudança de dia e mês
    patientService->serviceTime += patientService->imagingTests * procedures[4]->averageTime;
    patientService->state++;
    procedures[4]->units.push(patientService);

    events.push(Event(
      patientService->id,
      clock.hour + (patientService->imagingTests * procedures[4]->averageTime), // Ajustar pra mudança de dia e mês
      patientService->day,
      patientService->month,
      patientService->year
    ));
  }
} 

void Schedule::HandleMedicalSupplies(Clock clock) {
  Patient* patientService = nullptr;
  
  if (!serviceSevereQueue.Empty()) {
    patientService = serviceSevereQueue.Front();
    if (patientService->state == 12) serviceSevereQueue.Dequeue();
  } else if (!serviceModerateQueue.Empty()) {
    patientService = serviceModerateQueue.Front();
    if (patientService->state == 12) serviceModerateQueue.Dequeue();
  } else if (!serviceMildQueue.Empty()) {
    patientService = serviceMildQueue.Front();
    if (patientService->state == 12) serviceMildQueue.Dequeue();
  }

  if (patientService != nullptr && patientService->state == 12) {
    patientService->waitingTime += clock.hour - patientService->medicalSuppliesTime; // Ajustar pra mudança de dia e mês
    patientService->serviceTime += patientService->medicalSupplies * procedures[5]->averageTime;
    patientService->state++;
    procedures[5]->units.push(patientService);

    events.push(Event(
      patientService->id,
      clock.hour + (patientService->medicalSupplies * procedures[5]->averageTime), // Ajustar pra mudança de dia e mês
      patientService->day,
      patientService->month,
      patientService->year
    ));
  }
} 

void Schedule::Print() {
  StartSimulation();

  /*
  while (!events.empty()) {
    Event e = events.top();
    std::cout << "Evento: id " << e.id << " " << e.hour << "h, " << e.day << "/" << e.month << "/" << e.year << std::endl;
    events.pop();
  }*/
  for (int i = 0; i < numberPatients; i++) 
    patients[i]->Print();
  
  //std::cout << clock.day << " " << clock.hour << std::endl;
}