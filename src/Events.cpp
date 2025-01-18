#include "Events.hpp"

// Função de comparação
bool Events::compareEvents(const Event& a, const Event& b) {
  if (a.year != b.year) return a.year < b.year;
  if (a.month != b.month) return a.month < b.month;
  if (a.day != b.day) return a.day < b.day;
  return a.hour < b.hour;
}

void Events::swap(int a, int b) {
  Event temp = heap[a];
  heap[a] = heap[b];
  heap[b] = temp;
}

// Organiza o heap de cima para baixo
void Events::heapifyDown(int index) {
  int smallest = index;
  std::size_t left = 2 * index + 1;
  std::size_t right = 2 * index + 2;

  // Verifica o filho esquerdo
  if (left < heap.size() && compareEvents(heap[left], heap[smallest]))
    smallest = left;

  // Verifica o filho direito
  if (right < heap.size() && compareEvents(heap[right], heap[smallest]))
    smallest = right;

  // Se o menor elemento não for o nó atual
  if (smallest != index) {
    swap(index, smallest);
    heapifyDown(smallest);
  }
}

// Organiza o heap de baixo para cima
void Events::heapifyUp(int index) {
  int parent = (index - 1) / 2;

  if (index > 0 && compareEvents(heap[index], heap[parent])) {
    swap(index, parent);
    heapifyUp(parent);
  }
}

// Insere um evento no heap
void Events::push(const Event& evento) {
  heap.push_back(evento);
  heapifyUp(heap.size() - 1);
}

// Retorna o menor evento do heap
Event Events::top() {
  if (heap.empty()) throw runtime_error("Heap está vazio!");
  return heap[0];
}

// Remove o menor evento do heap
void Events::pop() {
  if (heap.empty()) throw runtime_error("Heap está vazio!");
  heap[0] = heap.back();
  heap.pop_back();
  heapifyDown(0);
}

// Verifica se o heap está vazio
bool Events::empty() const {
  return heap.empty();
}

// Retorna o tamanho do heap
int Events::size() const {
  return heap.size();
}
