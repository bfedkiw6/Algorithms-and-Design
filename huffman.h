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
  // Compares HuffmanNode pointers
  class CompareHuffmanNodes {
   public:
    bool operator()(HuffmanNode *node1, HuffmanNode *node2) {
      return *node1 < *node2;
    }
  };
  static void CountFrequency(std::string &file_contents,
                             std::array<int, 128> &freq_array);
  static void BuildHuffmanTree(
      std::array<int, 128> &freq_array,
      PQueue<HuffmanNode *, CompareHuffmanNodes> &huffman_tree);
  static void Encoding(HuffmanNode *node,
                       std::array<std::string, 128> &code_table,
                       std::string path, std::string &encoded_string);
  // Decompress Helpers
  static HuffmanNode *MakeNode(BinaryInputStream &bis);
  static HuffmanNode *RebuildTree(BinaryInputStream &bis);
  static void WriteEncodedString(BinaryInputStream &bis, std::ofstream &ofs,
                                 HuffmanNode *huffman_tree);
};

// To be completed below
void Huffman::CountFrequency(std::string &file_contents,
                             std::array<int, 128> &freq_array) {
  for (int i = 0; i < file_contents.size(); i++)
    freq_array[file_contents[i]]++;
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

void Huffman::Encoding(HuffmanNode *node,
                       std::array<std::string, 128> &code_table,
                       std::string path, std::string &encoded_string) {
  if (node->IsLeaf()) {
    code_table[node->data()] = path;
    encoded_string += '1';
    encoded_string += static_cast<char>(node->data());
  } else {
    encoded_string += '0';
    if (node->left())
      Encoding(node->left(), code_table, path + '0', encoded_string);
    if (node->right())
      Encoding(node->right(), code_table, path + '1', encoded_string);
  }

  delete node;
}

HuffmanNode *Huffman::MakeNode(BinaryInputStream &bis) {
  bool cur_bit = bis.GetBit();
  if (cur_bit)
    // Character node
    return new HuffmanNode(bis.GetChar(), 0);
  else
    // Internal node with next two nodes as its left and right children
    return new HuffmanNode(0, 0, MakeNode(bis), MakeNode(bis));
}

HuffmanNode *Huffman::RebuildTree(BinaryInputStream &bis) {
  if (bis.GetBit())
    // If only one unique character, no internal node
    return new HuffmanNode(bis.GetChar(), 0);
  else
    // Root will always be internal node otherwise
    return new HuffmanNode(0, 0, MakeNode(bis), MakeNode(bis));
}

// TODO(ethanbwang): Fix this function
void Huffman::WriteEncodedString(BinaryInputStream &bis, std::ofstream &ofs,
                                 HuffmanNode *huffman_tree) {
  // Get number of encoded characters
  int iterations = bis.GetInt();

  // Write characters to output file
  for (int i = 0; i < iterations; i++) {
    while (!huffman_tree->IsLeaf()) {  // while not leaf, traverse to find char
      if (bis.GetBit() == 0)
        n = n->left();
      if (bis.GetBit() == 1)
        n = n->right();
    }
    ofs << n->data();
  }
}

void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  std::string file_contents;
  std::array<int, 128> freq_array = {0};
  PQueue<HuffmanNode *, CompareHuffmanNodes> huffman_tree;
  std::string encoded_tree;
  std::array<std::string, 128> code_table = {""};

  // Read data into string
  char cur_char;
  while (ifs >> cur_char)
    file_contents += cur_char;
  // Gather necessary data
  CountFrequency(file_contents, freq_array);
  BuildHuffmanTree(freq_array, huffman_tree);
  Encoding(huffman_tree.Top(), code_table, "", encoded_tree);

  // Write to file (not in a function to not have to pass so many parameters)
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
  bos.PutInt(file_contents.size());
  // Write encoded characters
  for (int i = 0; i < file_contents.size(); i++) {
    cur_char = file_contents[i];
    std::string compressed_char = code_table[cur_char];
    for (int j = 0; j < compressed_char.size(); j++) {
      if (compressed_char[j] == '0')
        bos.PutBit(0);
      else
        bos.PutBit(1);
    }
  }

  bos.Close();
}

void Huffman::Decompress(std::ifstream &ifs, std::ofstream &ofs) {
  BinaryInputStream bis(ifs);
  HuffmanNode *huffman_tree = RebuildTree(bis);

  // Write to file
  WriteEncodedString(bis, ofs, huffman_tree);

  // Delete huffman tree
  std::queue<HuffmanNode *> deletion_queue;
  deletion_queue.push(huffman_tree);

  while (!deletion_queue.empty()) {
    HuffmanNode *cur_node = deletion_queue.front();
    if (cur_node->left())
      deletion_queue.push(cur_node->left());
    if (cur_node->right())
      deletion_queue.push(cur_node->right());

    delete cur_node;
    deletion_queue.pop();
  }
}

#endif  // HUFFMAN_H_
