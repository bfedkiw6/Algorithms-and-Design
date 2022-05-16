#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "bstream.h"
#include "pqueue.h"

class HuffmanNode {
 public:
  explicit HuffmanNode(char ch, size_t freq, HuffmanNode *left = nullptr,
                       HuffmanNode *right = nullptr)
      : ch_(ch), freq_(freq), left_(left), right_(right) {}

  bool IsLeaf() {
    // Node is a leaf if it doesn't have any children
    return left_ == nullptr && right_ == nullptr;
  }

  bool operator<(const HuffmanNode &n) const {
    // In case of equality, make it deterministic based on character
    if (freq_ == n.freq_)
      return ch_ < n.ch_;
    // Otherwise compare frequencies
    return freq_ < n.freq_;
  }

  size_t freq() { return freq_; }
  size_t data() { return ch_; }
  HuffmanNode *left() { return left_; }
  HuffmanNode *right() { return right_; }

 private:
  char ch_;
  size_t freq_;
  HuffmanNode *left_, *right_;
};

class Huffman {
 public:
  static void Compress(std::ifstream &ifs, std::ofstream &ofs);

  static void Decompress(std::ifstream &ifs, std::ofstream &ofs);

 private:
  // Helper methods...
  static void CountFrequency(std::ifstream &ifs,
                             std::array<int, 128> &freq_array);
  static void BuildHuffmanTree(std::array<int, 128> &freq_array,
                               PQueue<HuffmanNode> &huffman_tree);
};

// To be completed below
void Huffman::CountFrequency(std::ifstream &ifs,
                             std::array<int, 128> &freq_array) {
  char cur_char;

  while (ifs >> cur_char)
    freq_array[cur_char]++;
}

void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  std::array<int, 128> freq_array;
  PQueue<HuffmanNode> huffman_tree;

  CountFrequency(ifs, freq_array);
  BuildHuffmanTree(freq_array, huffman_tree);
}

void Huffman::Decompress(std::ifstream &ifs, std::ofstream &ofs) {}

#endif  // HUFFMAN_H_
