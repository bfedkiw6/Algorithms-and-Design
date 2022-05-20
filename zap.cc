#include <fstream>
#include <iostream>
#include <string>

#include "huffman.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <inputfile> <zapfile>\n";
    exit(1);
  }

  // Open files
  std::ifstream ifs(argv[1]);
  if (!ifs.is_open()) {
    std::cerr << "Error: cannot open input file " << argv[1] << '\n';
    exit(1);
  }

  std::ofstream ofs(argv[2],
                    std::ios::out | std::ios::trunc | std::ios::binary);
  if (!ofs.is_open()) {
    std::cerr << "Error: cannot open zap file " << argv[2] << '\n';
    exit(1);
  }

  // Compress
  Huffman::Compress(ifs, ofs);

  std::cout << "Compressed input file " << argv[1] << " into zap file "
            << argv[2] << '\n';

  ifs.close();
  ofs.close();
}
