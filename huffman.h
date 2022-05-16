#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cstddef>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "bstream.h"
#include "pqueue.h"

class HuffmanNode {
 public:
  explicit HuffmanNode(char ch, size_t freq,
                       HuffmanNode *left = nullptr,
                       HuffmanNode *right = nullptr)
      : ch_(ch), freq_(freq), left_(left), right_(right) { }


  bool IsLeaf() {
    // Node is a leaf if it doesn't have any children
    return left_ == nullptr && right_ == nullptr;
  }

  bool operator < (const HuffmanNode &n) const {
    // In case of equality, make it deterministic based on character
    if (freq_ == n.freq_)
      return ch_ < n.ch_;
    // Otherwise compare frequencies
    return freq_ < n.freq_;
  }

  size_t freq() { return freq_; }
  size_t data() { return ch_; }
  HuffmanNode* left() { return left_; }
  HuffmanNode* right() { return right_; }

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
  // this needs to be changed, possibly to a vector of pairs?
  std::vector<int> freq_vector;
  std::vector<HuffmanNode> vec;

  void CountFrequency(std::ifstream &ifs);
};

// To be completed below
void Huffman::CountFrequency(std::ifstream &ifs) {
  std::string line;

  while(std::getline(ifs, line)) {
    for(int i = 0; i < line.length(); ++i) {
      char ch = line[i];


    }
  }
}

static void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  CountFrequency(ifs);
  PQueue<HuffmanNode> pq;
}

static void Huffman::Decompress(std::ifstream &ifs, std::ofstream &ofs);

#endif  // HUFFMAN_H_
