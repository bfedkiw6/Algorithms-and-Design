#include <fstream>
#include <iostream>
#include <string>

#include "huffman.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <inputfile> <zapfile>\n";
    exit(1);
  }

  // Create variables and open files
  Huffman compressor;
  std::ifstream ifs(argv[1]);
  std::ofstream ofs(argv[2],
                    std::ios::out | std::ios::trunc | std::ios::binary);

  if (!ifs.is_open()) {
    std::cerr << "Error: cannot open input file " << argv[1] << '\n';
    exit(1);
  }
  if (!ofs.is_open()) {
    std::cerr << "Error: cannot open zap file " << argv[2] << '\n';
    exit(1);
  }

  // Compress
  compressor.Compress(ifs, ofs);

  std::cout << "Compressed input file " << argv[1] << " into zap file "
            << argv[2] << '\n';

  ifs.close();
  ofs.close();
}
