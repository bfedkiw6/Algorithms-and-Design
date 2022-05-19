#include <fstream>
#include <iostream>
#include <string>

#include "huffman.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <zapfile> <outputfile>\n";
    exit(1);
  }

  // Create variables and open files
  Huffman decompressor;
  std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
  // Truncate output
  std::ofstream ofs(argv[2], std::ios::out | std::ios::trunc);

  if (!ifs.is_open()) {
    std::cerr << "Error: cannot open zap file " << argv[1] << '\n';
    exit(1);
  }
  if (!ofs.is_open()) {
    std::cerr << "Error: cannot open output file " << argv[2] << '\n';
    exit(1);
  }

  // Decompress
  decompressor.Decompress(ifs, ofs);

  std::cout << "Decompressed zap file " << argv[1] << " into output file "
            << argv[2] << '\n';

  ifs.close();
  ofs.close();
}
