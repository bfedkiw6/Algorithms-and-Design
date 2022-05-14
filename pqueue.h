#ifndef PQUEUE_H_
#define PQUEUE_H_

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

template <typename T, typename C = std::less<T>>
class PQueue {
 public:
  // Constructor
  PQueue() {}
  // Return number of items in priority queue
  size_t Size();
  // Return top of priority queue
  T& Top();
  // Remove top of priority queue
  void Pop();
  // Insert item and sort priority queue
  void Push(const T& item);

 private:
  std::vector<T> items;
  size_t cur_size = 0;
  C cmp;

  // Helper methods for indices
  size_t Root() { return 0; }
  size_t Parent(size_t n) { return (n - 1) / 2; }
  size_t LeftChild(size_t n) { return 2 * n + 1; }
  size_t RightChild(size_t n) { return 2 * n + 2; }

  // Helper methods for node testing
  bool HasParent(size_t n) { return n != Root(); }
  bool IsNode(size_t n) { return n < cur_size; }

  // Helper methods for restructuring
  void PercolateUp(size_t n);
  void PercolateDown(size_t n);

  // Node comparison
  bool CompareNodes(size_t i, size_t j);
};

// To be completed below
template <typename T, typename C>
size_t PQueue<typename T, typename C>::Size() {
  return cur_size;
}

// From Professors Binary Heap Code
template <typename T, typename C>
T& PQueue<typename T, typename C>::Top() {
  if (!cur_size)
    throw std::underflow_error("Empty priority queue!");
  return items[0];
}

// From Professors Binary Heap Code
template <typename T, typename C>
void PQueue<typename T, typename C>::Pop() {
  if (!cur_size)
    throw std::underflow_error("Empty priority queue!");
  // Move last item back to root and reduce heap's size
  items[0] = std::move(items[cur_size--]);
  PercolateDown(0);
}

// From Professors Binary Heap Code, altered to fit no capacity
template <typename T, typename C>
void PQueue<typename T, typename C>::Push(const T& item) {
  // Insert at the end
  items[++cur_size] = std::move(item);
  // Percolate up
  PercolateUp(cur_size);
}

template <typename T, typename C>
void PQueue<typename T, typename C>::PercolateUp(size_t n) {
  while (HasParent(n) && cmp(items[n], items[Parent(n)])) {
    std::swap(items[Parent(n)], items[n]);
    n = Parent(n);
  }
}

template <typename T, typename C>
void PQueue<typename T, typename C>::PercolateDown(size_t n) {}

template <typename T, typename C>
bool PQueue<typename T, typename C>::CompareNodes(size_t i, size_t j) {}

#endif  // PQUEUE_H_
