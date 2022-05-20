#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>

#include "bstream.h"

TEST(BStream, OutputByBits) {
  std::string filename{"test_bstream_outputbybits"};

  // Write data to file
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  BinaryOutputStream bos(ofs);

  // Write ascii for 'B' 01000010
  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutBit(0);
  bos.PutBit(0);
  bos.PutBit(0);
  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutBit(0);

  // Write a few chars and ints
  bos.PutChar('o');
  bos.PutChar('o');
  bos.PutChar('!');
  bos.PutInt(4890320);
  bos.PutInt(28);

  bos.Close();
  ofs.close();

  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  unsigned char val[12];
  ifs.read(reinterpret_cast<char *>(val), sizeof(val));
  ifs.close();

  EXPECT_EQ(val[0], 0x42);
  EXPECT_EQ(val[1], 0x6F);
  EXPECT_EQ(val[2], 0x6F);
  EXPECT_EQ(val[3], 0x21);
  EXPECT_EQ(val[4], 0x0);
  EXPECT_EQ(val[5], 0x4A);
  EXPECT_EQ(val[6], 0x9E);
  EXPECT_EQ(val[7], 0xD0);
  EXPECT_EQ(val[8], 0x0);
  EXPECT_EQ(val[9], 0x0);
  EXPECT_EQ(val[10], 0x0);
  EXPECT_EQ(val[11], 0x1C);

  std::remove(filename.c_str());
}

TEST(BStream, InputByBits) {
  std::string filename{"test_bstream_inputbybits"};
  const unsigned char val[] = {
      0x58, 0x90, 0xab, 0x08, 0x00, 0x4e, 0xdb, 0x40,
  };
  // Equivalent in binary is:
  // 0101100010010000101010110000100000000000010011101101101101000000
  // ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

  // Write this to a file
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  ofs.write(reinterpret_cast<const char *>(val), sizeof(val));
  ofs.close();

  // Read it back in binary format
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  BinaryInputStream bis(ifs);

  // Make sure that we reading the binary in the correct order
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetChar(), 0x62);  // 01100010
  EXPECT_EQ(bis.GetChar(), 0x42);  // 01000010
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 0x58400276);  // 01011000010000000000001001110110

  ifs.close();

  std::remove(filename.c_str());
}

TEST(BStream, OutputAndInputByBits) {
  std::string filename{"test_bstream_output_and_input_by_bits"};

  // Write data to file
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  BinaryOutputStream bos(ofs);

  // Write a few chars and ints
  bos.PutChar('H');
  bos.PutChar('e');
  bos.PutChar('y');
  bos.PutChar('!');
  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutBit(1);
  bos.PutInt(2);
  bos.PutInt(37);
  bos.PutInt(47);

  bos.Close();
  ofs.close();

  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  BinaryInputStream bis(ifs);

  EXPECT_EQ(bis.GetChar(), 0x48);
  EXPECT_EQ(bis.GetChar(), 0x65);
  EXPECT_EQ(bis.GetChar(), 0x79);
  EXPECT_EQ(bis.GetChar(), 0x21);
  EXPECT_EQ(bis.GetBit(), 0x0);
  EXPECT_EQ(bis.GetBit(), 0x1);
  EXPECT_EQ(bis.GetBit(), 0x0);
  EXPECT_EQ(bis.GetBit(), 0x1);
  EXPECT_EQ(bis.GetBit(), 0x1);
  EXPECT_EQ(bis.GetInt(), 0x2);
  EXPECT_EQ(bis.GetInt(), 0x25);
  EXPECT_EQ(bis.GetInt(), 0x2F);
  ifs.close();

  std::remove(filename.c_str());
}

TEST(BStream, InputError) {
  std::string filename{"test_input_error"};

  // Create empty file
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  ofs.close();

  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  BinaryInputStream bis(ifs);

  EXPECT_THROW(bis.GetBit(), std::exception);
  EXPECT_THROW(bis.GetChar(), std::exception);
  EXPECT_THROW(bis.GetInt(), std::exception);

  ifs.close();
  std::remove(filename.c_str());
}

TEST(BStream, OutputIrregularly) {
  std::string filename{"test_output_irregularly"};

  // Write data to file
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  BinaryOutputStream bos(ofs);

  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutBit(0);
  bos.PutChar('D');
  bos.PutBit(0);
  bos.PutBit(0);
  bos.PutChar('K');
  bos.PutBit(0);
  bos.PutChar('E');
  bos.PutBit(1);
  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutBit(1);
  bos.PutChar('W');
  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutBit(0);
  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutInt(4890320);
  bos.PutBit(0);
  bos.PutInt(28);
  bos.PutBit(1);
  bos.PutBit(1);

  bos.Close();
  ofs.close();

  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  unsigned char val[15];
  ifs.read(reinterpret_cast<char *>(val), sizeof(val));
  ifs.close();

  // 01001000 10000010 01011001 00010110 11010101 11010010 00000000 10010101
  // 00111101 10100000 00000000 00000000 00000000 00011100 11000000
  EXPECT_EQ(val[0], 0x48);
  EXPECT_EQ(val[1], 0x82);
  EXPECT_EQ(val[2], 0x59);
  EXPECT_EQ(val[3], 0x16);
  EXPECT_EQ(val[4], 0xD5);
  EXPECT_EQ(val[5], 0xD2);
  EXPECT_EQ(val[6], 0x0);
  EXPECT_EQ(val[7], 0x95);
  EXPECT_EQ(val[8], 0x3D);
  EXPECT_EQ(val[9], 0xA0);
  EXPECT_EQ(val[10], 0x0);
  EXPECT_EQ(val[11], 0x0);
  EXPECT_EQ(val[12], 0x0);
  EXPECT_EQ(val[13], 0x1C);
  EXPECT_EQ(val[14], 0xC0);

  std::remove(filename.c_str());
}

TEST(BStream, InputIrregularly) {
  std::string filename{"test_input_irregularly"};
  const unsigned char val[] = {0x48, 0x82, 0x59, 0x16, 0xD5, 0xD2, 0x0, 0x95,
                               0x3D, 0xA0, 0x0,  0x0,  0x0,  0x1C, 0xC0};

  // Write data to file
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  ofs.write(reinterpret_cast<const char *>(val), sizeof(val));
  ofs.close();

  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  BinaryInputStream bis(ifs);

  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetChar(), 'D');
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetChar(), 'K');
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetChar(), 'E');
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetChar(), 'W');
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 4890320);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetInt(), 28);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_THROW(bis.GetBit(), std::exception);

  ifs.close();
  std::remove(filename.c_str());
}

TEST(BStream, OutputAndInputIrregularlySameOrder) {
  std::string filename{"test_output_and_input_irregularly_same_order"};

  // Write data to file
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  BinaryOutputStream bos(ofs);

  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutBit(0);
  bos.PutChar('D');
  bos.PutBit(0);
  bos.PutBit(0);
  bos.PutChar('K');
  bos.PutBit(0);
  bos.PutChar('E');
  bos.PutBit(1);
  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutBit(1);
  bos.PutChar('W');
  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutBit(0);
  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutInt(4890320);
  bos.PutBit(0);
  bos.PutInt(28);
  bos.PutBit(1);
  bos.PutBit(1);

  bos.Close();
  ofs.close();

  // Should read same thing back in
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  BinaryInputStream bis(ifs);

  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetChar(), 'D');
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetChar(), 'K');
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetChar(), 'E');
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetChar(), 'W');
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 4890320);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetInt(), 28);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_THROW(bis.GetBit(), std::exception);

  ifs.close();
  std::remove(filename.c_str());
}

TEST(BStream, BigInts) {
  std::string filename{"test_big_ints"};
  // Write data to file
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  BinaryOutputStream bos(ofs);

  bos.PutInt(584390);
  bos.PutInt(-3458902);
  bos.PutInt(20943854);
  bos.PutInt(584399832);
  bos.PutInt(-9835315);

  bos.Close();
  ofs.close();

  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  BinaryInputStream bis(ifs);

  EXPECT_EQ(bis.GetInt(), 584390);
  EXPECT_EQ(bis.GetInt(), -3458902);
  EXPECT_EQ(bis.GetInt(), 20943854);
  EXPECT_EQ(bis.GetInt(), 584399832);
  EXPECT_EQ(bis.GetInt(), -9835315);

  ifs.close();
  std::remove(filename.c_str());
}

TEST(BStream, OutputAndInputIrregularlyDifferentOrder) {
  std::string filename{"test_output_and_input_irregularly_different_order"};

  // Write data to file
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  BinaryOutputStream bos(ofs);

  // 01010001 00100000 00000000 00000000 00000011 10001001 01100000
  bos.PutBit(0);
  bos.PutBit(1);
  bos.PutChar('D');
  bos.PutBit(1);
  bos.PutInt(28);
  bos.PutChar('K');

  bos.Close();
  ofs.close();

  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  BinaryInputStream bis(ifs);

  EXPECT_EQ(bis.GetChar(), 0x51);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetChar(), 0x40);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetInt(), 0x1C4B);
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_THROW(bis.GetChar(), std::exception);
  EXPECT_THROW(bis.GetInt(), std::exception);
  EXPECT_THROW(bis.GetBit(), std::exception);

  ifs.close();
  std::remove(filename.c_str());
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
