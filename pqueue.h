#ifndef PQUEUE_H_
#define PQUEUE_H_

#include <algorithm>
#include <cassert>
#include <functional>
#include <utility>
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
size_t PQueue<T, C>::Size() {
  return cur_size;
}

// From Professor's Binary Heap Code
template <typename T, typename C>
T& PQueue<T, C>::Top() {
  if (!cur_size)
    throw std::underflow_error("Empty priority queue!");
  return items[0];
}

// From Professor's Binary Heap Code, altered for automatic resizing
template <typename T, typename C>
void PQueue<T, C>::Pop() {
  if (!cur_size)
    throw std::underflow_error("Empty priority queue!");
  // Move last item to root and reduce heap's size
  items[0] = std::move(items[--cur_size]);
  items.pop_back();
  PercolateDown(0);
}

// From Professor's Binary Heap Code, altered for automatic resizing
template <typename T, typename C>
void PQueue<T, C>::Push(const T& item) {
  // Insert at the end
  items.push_back(std::move(item));
  cur_size++;
  // Percolate up
  PercolateUp(cur_size - 1);
}

// Modified from Professor's code to use the cmp given in the template
template <typename T, typename C>
void PQueue<T, C>::PercolateUp(size_t n) {
  while (HasParent(n) && CompareNodes(n, Parent(n))) {
    std::swap(items[Parent(n)], items[n]);
    n = Parent(n);
  }
}

// Modified from Professor's code to use the cmp given in the template
template <typename T, typename C>
void PQueue<T, C>::PercolateDown(size_t n) {
  while (IsNode(LeftChild(n))) {
    size_t child = LeftChild(n);

    if (IsNode(RightChild(n)) && CompareNodes(RightChild(n), child))
      child = RightChild(n);

    if (CompareNodes(child, n))
      std::swap(items[child], items[n]);
    else
      break;

    n = child;
  }
}

// True if node at i is "less" than node at j
template <typename T, typename C>
bool PQueue<T, C>::CompareNodes(size_t i, size_t j) {
  return cmp(items[i], items[j]);
}

#endif  // PQUEUE_H_
