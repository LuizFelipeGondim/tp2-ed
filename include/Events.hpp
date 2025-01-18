#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <vector>
#include <stdexcept>
using namespace std;

struct Event {
  int hour;
  int day;
  int month;
  int year;

  Event(int h, int d, int m, int a) : hour(h), day(d), month(m), year(a) {}
};

class Events {
private:
  vector<Event> heap;

  void heapifyDown(int index);
  void heapifyUp(int index);
  bool compareEvents(const Event& a, const Event& b);
  void swap(int a, int b);

public:

  void push(const Event& evento);
  Event top();
  void pop();
  bool empty() const;
  int size() const;
};

#endif