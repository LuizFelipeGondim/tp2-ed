#include "queue.hpp"

template <typename T>
Queue<T>::Queue(int maxCapacity)
  : _front(0), _size(0), _capacity(maxCapacity) {
  _data = new T[_capacity];
}

template <typename T>
Queue<T>::~Queue() {
  delete[] _data;
}

template <typename T>
int Queue<T>::GetSize() const {
  return _size;
}

template <typename T>
bool Queue<T>::Empty() const {
  return _size == 0;
}

template <typename T>
void Queue<T>::Enqueue(T item) {
  if (_size == _capacity) {
    throw std::overflow_error("Queue is full!");
  }
  int position = (_front + _size) % _capacity;
  _data[position] = item;
  ++_size;
}

template <typename T>
T Queue<T>::Dequeue() {
  if (Empty()) {
    throw std::underflow_error("Queue is empty!");
  }
  T item = _data[_front];
  _front = (_front + 1) % _capacity;
  --_size;
  return item;
}

template <typename T>
void Queue<T>::Clear() {
  _front = 0;
  _size = 0;
}