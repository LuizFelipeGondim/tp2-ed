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
    waitingTime(0.0),
    serviceTime(0.0),
    state(1) {}

// Informa mês solicitado
std::string Patient::getMonthName(int month) {
  static const std::string months[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  return months[month - 1];
}

// Função auxiliar para obter o dia da semana seguindo o algoritmo de Zeller 
std::string Patient::getWeekdayName(int year, int month, int day) {
  if (month == 1 || month == 2) {
    month += 12;
    year--;
  }

  int k = year % 100;     
  int j = year / 100;      
  int dayOfWeek = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;

  // Ajustar para começar de domingo (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
  dayOfWeek = (dayOfWeek + 5) % 7;

  static const std::string weekDays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
  return weekDays[dayOfWeek];
}

// Função auxiliar para formatar a hora
std::string Patient::formatHour(double hour) {
  int h = static_cast<int>(hour);
  int m = static_cast<int>((hour - h) * 60 + 0.5);
  return (h < 10 ? "0" : "") + std::to_string(h) + ":" +
          (m < 10 ? "0" : "") + std::to_string(m) + ":00";
}

void Patient::print() {
  double totalTime = waitingTime + serviceTime;

  // Data de entrada
  std::string entryWeekday = getWeekdayName(year, month, day);
  std::string entryDate = entryWeekday + " " + getMonthName(month) + " " +
                          std::to_string(day) + " " + formatHour(hour) + " " +
                          std::to_string(year);

  // Data de saída
  int exitYear = year, exitMonth = month, exitDay = day;
  double exitHour = hour + totalTime;

  while (exitHour >= 24.0) {
      exitHour -= 24.0;
      exitDay++;

      int daysInMonth[] = {
          31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
      };

      int maxDay = daysInMonth[exitMonth - 1];

      if (exitMonth == 2 && ((exitYear % 4 == 0 && exitYear % 100 != 0) || (exitYear % 400 == 0)))
        maxDay = 29; // Ano bissexto

      if (exitDay > maxDay) {
        exitDay = 1;
        exitMonth++;
        if (exitMonth > 12) {
          exitMonth = 1;
          exitYear++;
        }
      }
  }
  std::string exitWeekday = getWeekdayName(exitYear, exitMonth, exitDay);
  std::string exitDate = exitWeekday + " " + getMonthName(exitMonth) + " " +
                          std::to_string(exitDay) + " " + formatHour(exitHour) + " " +
                          std::to_string(exitYear);

  std::cout << id << " " << entryDate << " " << exitDate << " "
            << std::fixed << std::setprecision(2)
            << totalTime << " " << serviceTime << " " << waitingTime << std::endl;
}
