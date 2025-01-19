#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <stdexcept>

template <typename T>
class Queue {
private:
  T* data; 
  int front;       
  int size;        
  int capacity;    

public:
  Queue(int maxCapacity = 5000);
  ~Queue();

  int GetSize() const;
  bool Empty() const;
  void Enqueue(T item);
  T Dequeue();
  T Front() const;
  void Clear();
};

#include "Queue.tpp"

#endif
