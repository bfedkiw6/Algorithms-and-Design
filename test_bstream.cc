#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>

#include "bstream.h"

#if 0
TEST(BStream, outputbybits) {
  std::string filename{"test_bstream_outputbybits"};
}
#endif

TEST(BStream, OutputMiscellaneous) {
  std::string filename{"test_bstream_outputmiscellaneous"};

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

TEST(BStream, inputbybits) {
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

TEST(BStream, writeandread) {
  std::string filename{"test_bstream_writeandread"};

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

#if 0
TEST(BStream, inputnormalfile) {
  std::string filename{"test_bstream_inputnormalfile"};
}
#endif

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
