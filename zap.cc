#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include <cerrno>

#include "huffman.h"

// From Insane Coding Website given in instructions
std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

int main(int argc, char *argv[]) {
  Huffman h;
  std::string filename{"truncated_input"};

  // Write file contents to truncated input
  std::string file_contents = get_file_contents(argv[1]);
  std::ofstream ifs(filename,
                    std::ios::in | std::ios::trunc | std::ios::binary);
  ifs.write(file_contents);
  ifs.close();

  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  std::ofstream ofs(argv[2], std::ios::out | std::ios::binary);

  h.Compress(ifs, ofs);

  std::cout << "Compressed input file " << argv[1] <<
            " into zap file " << argv[2] << std::endl;
}
