#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "Vector.hpp"
#include <stdexcept>

struct Event {
  int id;
  double hour;
  int day;
  int month;
  int year;

  Event(int id, double h, int d, int m, int a) : 
    id(id), 
    hour(h), 
    day(d), 
    month(m), 
    year(a){}

  Event() : 
    id(0), 
    hour(0), 
    day(0), 
    month(0), 
    year(0) {}
};

class Events {
private:
  Vector<Event> heap;

  void heapifyDown(int index);
  void heapifyUp(int index);
  bool compareEvents(const Event& a, const Event& b);
  void swap(int a, int b);

public:

  void push(const Event& event);
  Event top();
  void pop();
  bool empty() const;
  int size() const;
};

#endif