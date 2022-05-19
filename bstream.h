#ifndef BSTREAM_H_
#define BSTREAM_H_

#include <climits>
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
  char read_char = 0x00;

  for (int i = 0; i < CHAR_BIT; i++)
    read_char = read_char << 1 | GetBit();

  return read_char;
}

int BinaryInputStream::GetInt() {
  // To be completed
  int read_int = 0x00;

  for (int i = 0; i < sizeof(int) * CHAR_BIT; i++)
    read_int = read_int << 1 | GetBit();

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

// The & 0x1 checks the bit so that if the shift to the right
// induced any 1s to the left of the bit we are putting,
// the 1s get forced to 0
void BinaryOutputStream::PutChar(char byte) {
  // To be completed
  for (int i = CHAR_BIT - 1; i >= 0; i--) {
    bool bit = byte >> i & 0x1;
    PutBit(bit);
  }
}

void BinaryOutputStream::PutInt(int word) {
  // To be completed
  for (int i = sizeof(int) * CHAR_BIT - 1; i >= 0; i--) {
    bool bit = word >> i & 0x1;
    PutBit(bit);
  }
}

#endif  // BSTREAM_H_
