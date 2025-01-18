#include "Queue.hpp"

template <typename T>
Queue<T>::Queue(int maxCapacity)
  : front(0), size(0), capacity(maxCapacity) {
  data = new T[capacity];
}

template <typename T>
Queue<T>::~Queue() {
  delete[] data;
}

template <typename T>
int Queue<T>::GetSize() const {
  return size;
}

template <typename T>
bool Queue<T>::Empty() const {
  return size == 0;
}

template <typename T>
void Queue<T>::Enqueue(T item) {
  if (size == capacity) {
    throw std::overflow_error("Queue is full!");
  }
  int position = (front + size) % capacity;
  data[position] = item;
  ++size;
}

template <typename T>
T Queue<T>::Dequeue() {
  if (Empty()) {
    throw std::underflow_error("Queue is empty!");
  }
  T item = data[front];
  front = (front + 1) % capacity;
  --size;
  return item;
}

template <typename T>
void Queue<T>::Clear() {
  front = 0;
  size = 0;
}