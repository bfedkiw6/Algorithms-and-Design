#include <fstream>
#include <iostream>
#include <string>

#include "huffman.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <zapfile> <outputfile>\n";
    exit(1);
  }

  // Open files
  std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
  if (!ifs.is_open()) {
    std::cerr << "Error: cannot open zap file " << argv[1] << '\n';
    exit(1);
  }

  // Truncate output
  std::ofstream ofs(argv[2], std::ios::out | std::ios::trunc);
  if (!ofs.is_open()) {
    std::cerr << "Error: cannot open output file " << argv[2] << '\n';
    exit(1);
  }

  // Decompress
  Huffman::Decompress(ifs, ofs);

  std::cout << "Decompressed zap file " << argv[1] << " into output file "
            << argv[2] << '\n';

  ifs.close();
  ofs.close();
}
