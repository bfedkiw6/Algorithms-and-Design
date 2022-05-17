#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
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

  // Compress Helpers
  class CompareHuffmanNodes {
   public:
    bool operator()(HuffmanNode *node1, HuffmanNode *node2) {
      return *node1 < *node2;
    }
  };
  static void CountFrequency(std::ifstream &ifs,
                             std::array<int, 128> &freq_array);
  static void BuildHuffmanTree(
      std::array<int, 128> &freq_array,
      PQueue<HuffmanNode *, CompareHuffmanNodes> &huffman_tree);
  static void EncodeTree(
      PQueue<HuffmanNode *, CompareHuffmanNodes> &huffman_tree,
      std::string &encoded_tree);
  static void CreateCodeTable(HuffmanNode *node,
                              std::array<std::string, 128> &code_table,
                              std::string path);

  // Decompress Helpers
};

// To be completed below
void Huffman::CountFrequency(std::ifstream &ifs,
                             std::array<int, 128> &freq_array) {
  char cur_char;

  while (ifs >> cur_char)
    freq_array[cur_char]++;
}

void Huffman::BuildHuffmanTree(
    std::array<int, 128> &freq_array,
    PQueue<HuffmanNode *, CompareHuffmanNodes> &huffman_tree) {
  // Add Nodes
  for (int i = 0; i < 128; i++) {
    if (!freq_array[i])
      continue;

    huffman_tree.Push(new HuffmanNode(static_cast<char>(i), freq_array[i]));
  }

  // Tree building algorithm
  HuffmanNode *node1, *node2;
  while (huffman_tree.Size() != 1) {
    // Pop top 2 nodes
    node1 = huffman_tree.Top();
    huffman_tree.Pop();
    node2 = huffman_tree.Top();
    huffman_tree.Pop();

    huffman_tree.Push(
        new HuffmanNode(0, node1->freq() + node2->freq(), node1, node2));
  }
  assert(huffman_tree.Size() == 1);
}

void Huffman::EncodeTree(
    PQueue<HuffmanNode *, CompareHuffmanNodes> &huffman_tree,
    std::string &encoded_tree) {
  std::stack<HuffmanNode *> traversal_stack;
  traversal_stack.push(huffman_tree.Top());

  while (!traversal_stack.empty()) {
    HuffmanNode *n = traversal_stack.top();
    traversal_stack.pop();

    if (!n->IsLeaf()) {
      if (n->right())
        traversal_stack.push(n->right());
      if (n->left())
        traversal_stack.push(n->left());
      encoded_tree += '0';
    } else {
      encoded_tree += '1';
      encoded_tree += static_cast<char>(n->data());
    }
  }
}

void Huffman::CreateCodeTable(HuffmanNode *node,
                              std::array<std::string, 128> &code_table,
                              std::string path) {
  if (node->IsLeaf()) {
    code_table[node->data()] = path;
  } else {
    if (node->left()) {
      CreateCodeTable(node->left(), code_table, path + '0');
    }
    if (node->right()) {
      CreateCodeTable(node->right(), code_table, path + '1');
    }
  }
}

void DeleteHuffmanTree(HuffmanNode *root) {
  std::queue<HuffmanNode *> node_store;
  node_store.push(root);

  while (!node_store.empty()) {
    if (node_store.front()->left())
      node_store.push(node_store.front()->left());
    if (node_store.front()->right())
      node_store.push(node_store.front()->right());

    delete node_store.front();
    node_store.pop();
  }
}

void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  std::array<int, 128> freq_array = {0};
  PQueue<HuffmanNode *, CompareHuffmanNodes> huffman_tree;
  std::string encoded_tree;
  std::array<std::string, 128> code_table = {""};

  // Gather necessary data
  CountFrequency(ifs, freq_array);
  BuildHuffmanTree(freq_array, huffman_tree);
  EncodeTree(huffman_tree, encoded_tree);
  CreateCodeTable(huffman_tree.Top(), code_table, "");

  // Write to file
  BinaryOutputStream bos(ofs);

  // Write encoded tree
  for (int i = 0; i < encoded_tree.size(); i++) {
    if (encoded_tree[i] == '0') {
      bos.PutBit(0);
    } else {
      bos.PutBit(1);
      bos.PutChar(encoded_tree[++i]);
    }
  }
  // Write number of characters
  bos.PutInt(huffman_tree.Top()->freq());
  // Write encoded characters
  for (int i = 0; i < 128; i++) {
    if (freq_array[i] == 0)
      continue;

    for (int j = 0; j < freq_array[i]; j++) {
      std::string compressed_char = code_table[i];
      for (int k = 0; k < compressed_char.size(); k++) {
        if (compressed_char[k] == '0')
          bos.PutBit(0);
        else
          bos.PutBit(1);
      }
    }
  }

  // TODO: check if there are any memory leaks
  DeleteHuffmanTree(huffman_tree.Top());
  bos.Close();
}

void Huffman::Decompress(std::ifstream &ifs, std::ofstream &ofs) {}

#endif  // HUFFMAN_H_
