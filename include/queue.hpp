#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <stdexcept>

template <typename T>
class Queue {
private:
    T* _data; 
    int _front;       
    int _size;        
    int _capacity;    

public:
    Queue(int maxCapacity = 100);
    ~Queue();

    int GetSize() const;
    bool Empty() const;
    void Enqueue(T item);
    T Dequeue();
    void Clear();
};

#include "queue.tpp"

#endif
