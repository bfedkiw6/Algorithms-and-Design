#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <streambuf>
#include <string>
#include <utility>

#include "bstream.h"
#include "pqueue.h"

class HuffmanNode {
 public:
  explicit HuffmanNode(char ch, size_t freq,
                       std::unique_ptr<HuffmanNode> left = nullptr,
                       std::unique_ptr<HuffmanNode> right = nullptr)
      : ch_(ch), freq_(freq) {
    left_ = std::move(left);
    right_ = std::move(right);
  }

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
  HuffmanNode *left() { return left_.get(); }
  HuffmanNode *right() { return right_.get(); }

 private:
  char ch_;
  size_t freq_;
  std::unique_ptr<HuffmanNode> left_, right_;
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
    bool operator()(std::unique_ptr<HuffmanNode> &node1,
                    std::unique_ptr<HuffmanNode> &node2) {
      return *node1 < *node2;
    }
  };
  static void CountFrequency(std::string &file_contents,
                             std::array<int, 128> &freq_array);
  static std::unique_ptr<HuffmanNode> BuildHuffmanTree(
      std::array<int, 128> &freq_array);
  static void Encoding(HuffmanNode *node,
                       std::array<std::string, 128> &code_table,
                       std::string path, std::string &encoded_string);
  // Decompress Helpers
  static std::unique_ptr<HuffmanNode> MakeNode(BinaryInputStream &bis);
  static std::unique_ptr<HuffmanNode> RebuildTree(BinaryInputStream &bis);
  static void WriteEncodedString(BinaryInputStream &bis, std::ofstream &ofs,
                                 HuffmanNode *huffman_tree);
};

// To be completed below
void Huffman::CountFrequency(std::string &file_contents,
                             std::array<int, 128> &freq_array) {
  for (size_t i = 0; i < file_contents.size(); i++)
    freq_array[file_contents[i]]++;
}

std::unique_ptr<HuffmanNode> Huffman::BuildHuffmanTree(
    std::array<int, 128> &freq_array) {
  PQueue<std::unique_ptr<HuffmanNode>, CompareHuffmanNodes> huffman_tree;
  // Add Nodes
  for (int i = 0; i < 128; i++) {
    if (!freq_array[i])
      continue;

    std::unique_ptr<HuffmanNode> node(std::unique_ptr<HuffmanNode>(
        new HuffmanNode(static_cast<char>(i), freq_array[i])));
    huffman_tree.Push<HuffmanNode>(std::move(node));
  }

  // Tree building algorithm
  std::unique_ptr<HuffmanNode> node1, node2;
  while (huffman_tree.Size() != 1) {
    // Pop top 2 nodes
    std::swap(node1, huffman_tree.Top());
    huffman_tree.Pop();
    std::swap(node2, huffman_tree.Top());
    huffman_tree.Pop();

    std::unique_ptr<HuffmanNode> internal_node(new HuffmanNode(
        0, node1->freq() + node2->freq(), std::move(node1), std::move(node2)));
    huffman_tree.Push<HuffmanNode>(std::move(internal_node));
  }
  assert(huffman_tree.Size() == 1);

  return std::move(huffman_tree.Top());
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
}

std::unique_ptr<HuffmanNode> Huffman::MakeNode(BinaryInputStream &bis) {
  bool cur_bit = bis.GetBit();
  if (cur_bit)
    // Character node
    return std::unique_ptr<HuffmanNode>(new HuffmanNode(bis.GetChar(), 0));
  else
    // Internal node with next two nodes as its left and right children
    return std::unique_ptr<HuffmanNode>(
        new HuffmanNode(0, 0, MakeNode(bis), MakeNode(bis)));
}

std::unique_ptr<HuffmanNode> Huffman::RebuildTree(BinaryInputStream &bis) {
  if (bis.GetBit())
    // If only one unique character, no internal node
    return std::unique_ptr<HuffmanNode>(new HuffmanNode(bis.GetChar(), 0));
  else
    // Root will always be internal node otherwise
    return std::unique_ptr<HuffmanNode>(
        new HuffmanNode(0, 0, MakeNode(bis), MakeNode(bis)));
}

void Huffman::WriteEncodedString(BinaryInputStream &bis, std::ofstream &ofs,
                                 HuffmanNode *huffman_tree) {
  // Get number of encoded characters
  int num_chars = bis.GetInt();

  // Write characters to output file
  for (int i = 0; i < num_chars; i++) {
    HuffmanNode *cur_node = huffman_tree;

    while (!cur_node->IsLeaf()) {
      if (bis.GetBit())
        cur_node = cur_node->right();
      else
        cur_node = cur_node->left();
    }
    ofs << static_cast<char>(cur_node->data());
  }
}

void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  std::string file_contents;
  std::array<int, 128> freq_array = {0};
  std::string encoded_tree;
  std::array<std::string, 128> code_table = {""};

  // Read data into string (taken from website given)
  file_contents = std::string(std::istreambuf_iterator<char>(ifs),
                              std::istreambuf_iterator<char>());
  // Gather necessary data
  CountFrequency(file_contents, freq_array);
  std::unique_ptr<HuffmanNode> huffman_tree = BuildHuffmanTree(freq_array);
  Encoding(huffman_tree.get(), code_table, "", encoded_tree);

  // Write to file (not in a function to not have to pass so many parameters)
  BinaryOutputStream bos(ofs);
  // Write encoded tree
  for (size_t i = 0; i < encoded_tree.size(); i++) {
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
  for (size_t i = 0; i < file_contents.size(); i++) {
    char cur_char = file_contents[i];
    std::string compressed_char = code_table[cur_char];
    for (size_t j = 0; j < compressed_char.size(); j++) {
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

  // Rebuild tree
  std::unique_ptr<HuffmanNode> huffman_tree = RebuildTree(bis);
  // Write to file
  WriteEncodedString(bis, ofs, huffman_tree.get());
}

#endif  // HUFFMAN_H_
