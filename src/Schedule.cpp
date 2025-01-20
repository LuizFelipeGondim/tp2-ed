#include "Schedule.hpp"

// Efetua a leitura dos dados
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

// Inicia a simulação do escalonador adicionando um evento pra cada paciente
// Depois ele vai analisando os estados do paciente e as situações das filas
// Dependendo desses fatores, um novo evento é adicionado no escalonador
// A simulação acaba quando não houver mais eventos a serem analisados ou as filas estiverem vazias
void Schedule::startSimulation() {
  bool finishSimulation = false;

  if (patients.getSize() == 0)
    finishSimulation = true;
  
  // Inicia eventos com os pacientes
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

    // Altera o tempo do relógio
    Event event = events.top();
    clock.hour = event.hour;
    clock.day = event.day;
    clock.month = event.month;
    clock.year = event.year;

    // Seleciona paciente do evento
    Patient* patient;
    for (int i = 0; i < patients.getSize(); i++)
      if (event.id == patients[i]->id) 
        patient = patients[i];

    // Adiciona paciente na fila de triagem
    // O tempo de entrada na fila também é calculado
    if (patient->state == 1) {
      patient->state++;
      patient->triageTime.hour = clock.hour;
      patient->triageTime.day = clock.day;
      patient->triageTime.month = clock.month;
      patient->triageTime.year = clock.year;
      triageQueue.enqueue(patient);
    }
    
    if (patient->state == 3) {

      // Verifica se algum paciente já terminou a triagem.
      // Se terminou, ele é mandado para a fila de espera do atendimento.
      // O tempo de entrada na fila também é calculado
      patient->state++;
      patient->patientCareTime.hour = clock.hour;
      patient->patientCareTime.day = clock.day;
      patient->patientCareTime.month = clock.month;
      patient->patientCareTime.year = clock.year;

      if (patient->urgency == 0) {
        patientCareMildQueue.enqueue(patient);
      } else if (patient->urgency == 1) {
        patientCareModerateQueue.enqueue(patient);
      } else if (patient->urgency == 2) {
        patientCareSevereQueue.enqueue(patient);
      } 

      for (int i = 0; i < procedures[0]->units.getSize(); i++)
        if (patient->id == procedures[0]->units[i]->id)
          procedures[0]->units.pop(i);
    }

    // Verifica se o paciente recebeu alta, caso não, ele é encaminhado para sua fila
    // O tempo de entrada na fila também é calculado
    if (patient->state == 5) {

      for (int i = 0; i < procedures[1]->units.getSize(); i++)
        if (patient->id == procedures[1]->units[i]->id)
          procedures[1]->units.pop(i);

      if (patient->discharged) {
        patient->state = 14;
      } else if (patient->hospitalMeasures > 0) { // Verifica se paciente pode pular estado
        patient->state++;
        patient->hospitalMeasuresTime.hour = clock.hour;
        patient->hospitalMeasuresTime.day = clock.day;
        patient->hospitalMeasuresTime.month = clock.month;
        patient->hospitalMeasuresTime.year = clock.year;

        if (patient->urgency == 0) {
          serviceMildQueue.enqueue(patient);
        } else if (patient->urgency == 1) {
          serviceModerateQueue.enqueue(patient);
        } else if (patient->urgency == 2) {
          serviceSevereQueue.enqueue(patient);
        }
      } else {
        patient->state = 7;
      }
    }

    // Encaminha e calcula entrada do paciente na fila de serviços, com estado 7
    if (patient->state == 7) {
      for (int i = 0; i < procedures[2]->units.getSize(); i++)
        if (patient->id == procedures[2]->units[i]->id)
          procedures[2]->units.pop(i);

      if (patient->laboratoryTests > 0) { // Verifica se paciente pode pular estado
        patient->state++;
        patient->laboratoryTestsTime.hour = clock.hour;
        patient->laboratoryTestsTime.day = clock.day;
        patient->laboratoryTestsTime.month = clock.month;
        patient->laboratoryTestsTime.year = clock.year;

        if (patient->urgency == 0) {
          serviceMildQueue.enqueue(patient);
        } else if (patient->urgency == 1) {
          serviceModerateQueue.enqueue(patient);
        } else if (patient->urgency == 2) {
          serviceSevereQueue.enqueue(patient);
        }
      } else {
        patient->state = 9;
      }
    }

    // Encaminha e calcula entrada do paciente na fila de serviços, com estado 9
    if (patient->state == 9) {
      for (int i = 0; i < procedures[3]->units.getSize(); i++)
        if (patient->id == procedures[3]->units[i]->id)
          procedures[3]->units.pop(i);

      if (patient->imagingTests > 0) { // Verifica se paciente pode pular estado
        patient->state++;
        patient->imagingTestsTime.hour = clock.hour;
        patient->imagingTestsTime.day = clock.day;
        patient->imagingTestsTime.month = clock.month;
        patient->imagingTestsTime.year = clock.year;

        if (patient->urgency == 0) {
          serviceMildQueue.enqueue(patient);
        } else if (patient->urgency == 1) {
          serviceModerateQueue.enqueue(patient);
        } else if (patient->urgency == 2) {
          serviceSevereQueue.enqueue(patient);
        }
      } else {
        patient->state = 11;
      }
    }

    // Encaminha e calcula entrada do paciente na fila de serviços, com estado 12
    if (patient->state == 11) {
      for (int i = 0; i < procedures[4]->units.getSize(); i++)
        if (patient->id == procedures[4]->units[i]->id)
          procedures[4]->units.pop(i);

      if (patient->medicalSupplies > 0) { // Verifica se paciente pode pular estado
        patient->state++;
        patient->medicalSuppliesTime.hour = clock.hour;
        patient->medicalSuppliesTime.day = clock.day;
        patient->medicalSuppliesTime.month = clock.month;
        patient->medicalSuppliesTime.year = clock.year;

        if (patient->urgency == 0) {
          serviceMildQueue.enqueue(patient);
        } else if (patient->urgency == 1) {
          serviceModerateQueue.enqueue(patient);
        } else if (patient->urgency == 2) {
          serviceSevereQueue.enqueue(patient);
        }
      } else {
        patient->state = 13;
      }
    }

    // Alta hospitalar
    if (patient->state == 13) {
      for (int i = 0; i < procedures[5]->units.getSize(); i++)
        if (patient->id == procedures[5]->units[i]->id)
          procedures[5]->units.pop(i);

      patient->state++;
    }  

    // Verifica se tem espaço no serviço de triagem, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[0]->hasSpace();
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        if (!triageQueue.empty()) {
          handleTriageQueue(clock);
        }
      }
    }
    
    // Verifica se tem espaço no serviço de atendimento, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[1]->hasSpace();
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        handlePatientCare(clock);
      }
    }

    // Verifica se tem espaço no serviço de medidas hospitalares, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[2]->hasSpace();
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        handleHospitalMeasures(clock);
      }
    }

    // Verifica se tem espaço no serviço de testes laboratoriais, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[3]->hasSpace();
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        handleLaboratoryTests(clock);
      }
    }
    
    // Verifica se tem espaço no serviço de testes de imagem, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[4]->hasSpace();
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        handleImagingTests(clock);
      }
    }

    // Verifica se tem espaço no serviço de suprimentos médicos, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[5]->hasSpace();
    if (hasSpace > 0) {
      for (int i = 0; i < hasSpace; i++) {
        handleMedicalSupplies(clock);
      }
    }

    events.pop();

    // Verifica se ainda há eventos e se hś alguem em alguma fila
    if (events.empty()) {

      if (!triageQueue.empty()) {
        handleTriageQueue(clock);
      } else if (
        !patientCareMildQueue.empty() || 
        !patientCareModerateQueue.empty() || 
        !patientCareSevereQueue.empty()
      ) {
        handlePatientCare(clock);
      } else if (
        !serviceMildQueue.empty() || 
        !serviceModerateQueue.empty() || 
        !serviceSevereQueue.empty()
      ) {
        handleHospitalMeasures(clock);
        handleLaboratoryTests(clock);
        handleImagingTests(clock);
        handleMedicalSupplies(clock);
      } else {
        finishSimulation = true;
      }

    }
  }
}

// Verifica se a fila de triagem está vazia.
// Se estiver, o paciente é retirado da fila para atualizar suas estatísticas 
// Depois um novo evento é inserido
void Schedule::handleTriageQueue(Clock clock) {
  Patient* patientTriage = triageQueue.dequeue();

  patientTriage->waitingTime += convertToTotalHours(clock) - 
                                  convertToTotalHours(patientTriage->triageTime);
  patientTriage->serviceTime += procedures[0]->averageTime;
  patientTriage->state++;
  procedures[0]->units.push(patientTriage);

  insertEvent(
    patientTriage->id,
    clock,
    procedures[0]->averageTime
  );
}

// Verifica se as filas de atendimento estão vazias
// Se estiver, ele é retirado da fila para atualizar suas estatísticas e depois um novo evento é inserido
void Schedule::handlePatientCare(Clock clock) {
  Patient* patientCare = nullptr;
  
  if (!patientCareSevereQueue.empty()) {
    patientCare = patientCareSevereQueue.dequeue();
  } else if (!patientCareModerateQueue.empty()) {
    patientCare = patientCareModerateQueue.dequeue();
  } else if (!patientCareMildQueue.empty()) {
    patientCare = patientCareMildQueue.dequeue();
  }

  if (patientCare != nullptr) {
    patientCare->waitingTime += convertToTotalHours(clock) - 
                                  convertToTotalHours(patientCare->patientCareTime);
    patientCare->serviceTime += procedures[1]->averageTime;
    patientCare->state++;
    procedures[1]->units.push(patientCare);

    insertEvent(
      patientCare->id,
      clock,
      procedures[1]->averageTime
    );
  }
}

// Verifica se as filas de serviço estão vazias e o estado do paciente é igual a 6
// Se for, ele é retirado da fila para atualizar suas estatísticas e depois um novo evento é inserido
void Schedule::handleHospitalMeasures(Clock clock) {
  Patient* patientService = nullptr;
        
  if (!serviceSevereQueue.empty()) {
    patientService = serviceSevereQueue.front();
    if (patientService->state == 6) serviceSevereQueue.dequeue();
  } else if (!serviceModerateQueue.empty()) {
    patientService = serviceModerateQueue.front();
    if (patientService->state == 6) serviceModerateQueue.dequeue();
  } else if (!serviceMildQueue.empty()) {
    patientService = serviceMildQueue.front();
    if (patientService->state == 6) serviceMildQueue.dequeue();
  }

  if (patientService != nullptr && patientService->state == 6) {
    patientService->waitingTime += convertToTotalHours(clock) - 
                                  convertToTotalHours(patientService->hospitalMeasuresTime);
    patientService->serviceTime += patientService->hospitalMeasures * procedures[2]->averageTime;
    patientService->state++;
    procedures[2]->units.push(patientService);

    insertEvent(
      patientService->id,
      clock,
      (patientService->hospitalMeasures * procedures[2]->averageTime)
    );
  }
} 

// Verifica se as filas de serviço estão vazias e o estado do paciente é igual a 8
// Se for, ele é retirado da fila para atualizar suas estatísticas e depois um novo evento é inserido
void Schedule::handleLaboratoryTests(Clock clock) {
  Patient* patientService = nullptr;
  
  if (!serviceSevereQueue.empty()) {
    patientService = serviceSevereQueue.front();
    if (patientService->state == 8) serviceSevereQueue.dequeue();
  } else if (!serviceModerateQueue.empty()) {
    patientService = serviceModerateQueue.front();
    if (patientService->state == 8) serviceModerateQueue.dequeue();
  } else if (!serviceMildQueue.empty()) {
    patientService = serviceMildQueue.front();
    if (patientService->state == 8) serviceMildQueue.dequeue();
  }

  if (patientService != nullptr && patientService->state == 8) {
    patientService->waitingTime += convertToTotalHours(clock) - 
                                  convertToTotalHours(patientService->laboratoryTestsTime);
    patientService->serviceTime += patientService->laboratoryTests * procedures[3]->averageTime;
    patientService->state++;
    procedures[3]->units.push(patientService);

    insertEvent(
      patientService->id,
      clock,
      (patientService->laboratoryTests * procedures[3]->averageTime)
    );
  }
} 

// Verifica se as filas de serviço estão vazias e o estado do paciente é igual a 10
// Se for, ele é retirado da fila para atualizar suas estatísticas e depois um novo evento é inserido
void Schedule::handleImagingTests(Clock clock) {
  Patient* patientService = nullptr;
  
  if (!serviceSevereQueue.empty()) {
    patientService = serviceSevereQueue.front();
    if (patientService->state == 10) serviceSevereQueue.dequeue();
  } else if (!serviceModerateQueue.empty()) {
    patientService = serviceModerateQueue.front();
    if (patientService->state == 10) serviceModerateQueue.dequeue();
  } else if (!serviceMildQueue.empty()) {
    patientService = serviceMildQueue.front();
    if (patientService->state == 10) serviceMildQueue.dequeue();
  }

  if (patientService != nullptr && patientService->state == 10) {
    patientService->waitingTime += convertToTotalHours(clock) - 
                                  convertToTotalHours(patientService->imagingTestsTime);
    patientService->serviceTime += patientService->imagingTests * procedures[4]->averageTime;
    patientService->state++;
    procedures[4]->units.push(patientService);

    insertEvent(
      patientService->id,
      clock,
      (patientService->imagingTests * procedures[4]->averageTime)
    );

  }
} 

// Verifica se as filas de serviço estão vazias e o estado do paciente é igual a 12
// Se for, ele é retirado da fila para atualizar suas estatísticas e depois um novo evento é inserido
void Schedule::handleMedicalSupplies(Clock clock) {
  Patient* patientService = nullptr;
  
  if (!serviceSevereQueue.empty()) {
    patientService = serviceSevereQueue.front();
    if (patientService->state == 12) serviceSevereQueue.dequeue();
  } else if (!serviceModerateQueue.empty()) {
    patientService = serviceModerateQueue.front();
    if (patientService->state == 12) serviceModerateQueue.dequeue();
  } else if (!serviceMildQueue.empty()) {
    patientService = serviceMildQueue.front();
    if (patientService->state == 12) serviceMildQueue.dequeue();
  }

  if (patientService != nullptr && patientService->state == 12) {
    patientService->waitingTime += convertToTotalHours(clock) - 
                                  convertToTotalHours(patientService->medicalSuppliesTime);
    patientService->serviceTime += patientService->medicalSupplies * procedures[5]->averageTime;
    patientService->state++;
    procedures[5]->units.push(patientService);

    insertEvent(
      patientService->id,
      clock,
      (patientService->medicalSupplies * procedures[5]->averageTime)
    );
  }
} 

// Insere um evento no escalonador fazendo as devidas conversões de tempo 
void Schedule::insertEvent(int id, Clock clock, double increment) {
  const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  clock.hour += increment;

  while (clock.hour >= 24.0) {
    clock.hour -= 24.0;
    clock.day++;
  }

  while (true) {
    int daysInCurrentMonth = daysInMonth[clock.month - 1];
    if (clock.month == 2 && isLeapYear(clock.year))
      daysInCurrentMonth = 29;

    if (clock.day <= daysInCurrentMonth)
      break;

    clock.day -= daysInCurrentMonth;
    clock.month++;

    if (clock.month > 12) {
      clock.month = 1;
      clock.year++;
    }
  }

  events.push(Event(
    id,
    clock.hour,
    clock.day,
    clock.month,
    clock.year
  ));
}

// Verifica se o ano informado é bissexto
bool Schedule::isLeapYear(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Converte a data do clock em horas
double Schedule::convertToTotalHours(Clock date) {
  const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  double totalHours = 0.0;

  for (int year = 2000; year < date.year; ++year) 
    totalHours += (isLeapYear(year) ? 366 : 365) * 24;

  for (int month = 1; month < date.month; ++month) {
    int daysInCurrentMonth = daysInMonth[month - 1];
    if (month == 2 && isLeapYear(date.year))
      daysInCurrentMonth = 29;

    totalHours += daysInCurrentMonth * 24;
  }
  totalHours += (date.day - 1) * 24 + date.hour;

  return totalHours;
}

// Sobrecarga da função anterior para converter o tipo de data Date
double Schedule::convertToTotalHours(Date date) {
  const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  double totalHours = 0.0;

  for (int year = 2000; year < date.year; ++year) 
    totalHours += (isLeapYear(year) ? 366 : 365) * 24;

  for (int month = 1; month < date.month; ++month) {
    int daysInCurrentMonth = daysInMonth[month - 1];
    if (month == 2 && isLeapYear(date.year))
      daysInCurrentMonth = 29;

    totalHours += daysInCurrentMonth * 24;
  }
  totalHours += (date.day - 1) * 24 + date.hour;

  return totalHours;
}

// Mostra as estatísticas
void Schedule::print() {
  startSimulation();
  for (int i = 0; i < numberPatients; i++) 
    patients[i]->print();
}