#ifndef BSTREAM_H_
#define BSTREAM_H_

#include <cstddef>
#include <fstream>
#include <iostream>

class BinaryInputStream {
 public:
  explicit BinaryInputStream(std::ifstream &ifs);

  bool GetBit();
  char GetChar();
  int GetInt();

 private:
  std::ifstream &ifs;
  char buffer = 0;
  size_t avail = 0;

  // Helpers
  void RefillBuffer();
};

BinaryInputStream::BinaryInputStream(std::ifstream &ifs) : ifs(ifs) {}

void BinaryInputStream::RefillBuffer() {
  // Read the next byte from the input stream
  ifs.get(buffer);
  if (ifs.gcount() != 1)
    throw std::underflow_error("No more characters to read");
  avail = 8;
}

bool BinaryInputStream::GetBit() {
  bool bit;

  if (!avail)
    RefillBuffer();

  avail--;
  // Check if bit is 0 or 1, doesn't shift buffer
  bit = ((buffer >> avail) & 1) == 1;

#if 0  // Switch to 1 for debug purposes
  if (bit)
    std::cout << '1' << std::endl;
  else
    std::cout << '0' << std::endl;
#endif

  return bit;
}

char BinaryInputStream::GetChar() {
  // To be completed
  char current_byte;
  bool curr_bit;
  char read_byte = 0x00;

  for (int i = 0; i < 8; i++) {
    curr_bit = GetBit();
    if (!curr_bit)
      current_byte = 0x00;
    else
      current_byte = 0x80 >> i;

    read_byte = read_byte | current_byte;
    buffer = buffer << 1;
  }

  return read_byte;
}

char BinaryInputStream::GetChar() {
  // To be completed
  char read_char = 0x00;

  for (int i = 0; i < CHAR_BIT; i++)
    read_char = read_char << 1 | GetBit();

  return read_char;
}

int BinaryInputStream::GetInt() {
  // To be completed
  int current_byte;
  bool curr_bit;
  int read_byte = 0x00000000;

  for (int i = 0; i < 32; i++) {
    curr_bit = GetBit();
    if (curr_bit == 0)
      current_byte = 0x00000000;
    else
      current_byte = 0x80000000 >> i;

    read_byte = read_byte | current_byte;
    buffer = buffer << 1;
  }

  return read_byte;
}

int BinaryInputStream::GetInt() {
  // To be completed
  int read_int = 0x00;

  for (int i = 0; i < sizeof(int); i++)
    read_int = read_int << CHAR_BIT | GetChar();

  return read_int;
}

class BinaryOutputStream {
 public:
  explicit BinaryOutputStream(std::ofstream &ofs);
  ~BinaryOutputStream();

  void Close();

  void PutBit(bool bit);
  void PutChar(char byte);
  void PutInt(int word);

 private:
  std::ofstream &ofs;
  char buffer = 0;
  size_t count = 0;

  // Helpers
  void FlushBuffer();
};

BinaryOutputStream::BinaryOutputStream(std::ofstream &ofs) : ofs(ofs) {}

BinaryOutputStream::~BinaryOutputStream() { Close(); }

void BinaryOutputStream::Close() { FlushBuffer(); }

void BinaryOutputStream::FlushBuffer() {
  // Nothing to flush
  if (!count)
    return;

  // If buffer isn't complete, pad with 0s before writing
  if (count > 0)
    buffer <<= (8 - count);

  // Write to output stream
  ofs.put(buffer);

  // Reset buffer
  buffer = 0;
  count = 0;
}

void BinaryOutputStream::PutBit(bool bit) {
  // Make some space and add bit to buffer
  buffer <<= 1;
  if (bit)
    buffer |= 1;

  // If buffer is full, write it
  if (++count == 8)
    FlushBuffer();
}

void BinaryOutputStream::PutChar(char byte) {
  // To be completed
}

void BinaryOutputStream::PutInt(int word) {
  // To be completed
}

#endif  // BSTREAM_H_
