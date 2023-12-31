#include <gtest/gtest.h>

#include <functional>

#include "./pqueue.h"

TEST(PQueue, Push) {
  PQueue<int> pq;

  pq.Push(28);
  EXPECT_EQ(pq.Top(), 28);
  pq.Push(19);
  EXPECT_EQ(pq.Top(), 19);
  pq.Push(50);
  EXPECT_EQ(pq.Top(), 19);
  pq.Push(20);
  EXPECT_EQ(pq.Top(), 19);
  pq.Push(3);
  EXPECT_EQ(pq.Top(), 3);
  pq.Push(1);
  EXPECT_EQ(pq.Top(), 1);
  EXPECT_EQ(pq.Size(), 6);
}

TEST(PQueue, Top) {
  PQueue<int> pq;

  EXPECT_THROW(pq.Top(), std::exception);

  pq.Push(28);
  pq.Push(19);
  pq.Push(50);
  pq.Push(20);
  pq.Push(3);
  pq.Push(1);
  EXPECT_EQ(pq.Top(), 1);
}

TEST(PQueue, Pop) {
  PQueue<int> pq;

  EXPECT_THROW(pq.Pop(), std::exception);

  pq.Push(28);
  pq.Push(19);
  pq.Push(50);
  pq.Push(20);
  pq.Push(3);
  pq.Push(1);
  EXPECT_EQ(pq.Top(), 1);
  EXPECT_EQ(pq.Size(), 6);

  pq.Pop();
  EXPECT_EQ(pq.Top(), 3);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 19);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 20);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 28);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 50);
  pq.Pop();

  EXPECT_THROW(pq.Top(), std::exception);
  EXPECT_EQ(pq.Size(), 0);
}

TEST(PQueue, Size) {
  PQueue<int> pq;

  EXPECT_EQ(pq.Size(), 0);

  pq.Push(28);
  EXPECT_EQ(pq.Size(), 1);
  pq.Push(19);
  pq.Push(50);
  pq.Push(20);
  EXPECT_EQ(pq.Size(), 4);
  pq.Push(3);
  pq.Push(1);
  EXPECT_EQ(pq.Size(), 6);

  pq.Pop();
  EXPECT_EQ(pq.Size(), 5);
  pq.Pop();
  pq.Pop();
  EXPECT_EQ(pq.Size(), 3);
  pq.Pop();
  pq.Pop();
  EXPECT_EQ(pq.Size(), 1);
  pq.Pop();
  EXPECT_EQ(pq.Size(), 0);
}

TEST(PQueue, PushPop) {
  PQueue<int> pq;

  pq.Push(28);
  pq.Push(56);
  pq.Push(2);
  EXPECT_EQ(pq.Top(), 2);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 28);
  pq.Push(90);
  pq.Push(1432);
  EXPECT_EQ(pq.Top(), 28);
  pq.Pop();
  pq.Push(1);
  pq.Push(0);
  EXPECT_EQ(pq.Top(), 0);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 1);
  EXPECT_EQ(pq.Size(), 4);
}

TEST(PQueue, Duplicates) {
  PQueue<int> pq;

  pq.Push(28);
  pq.Push(28);
  pq.Push(32);
  pq.Push(134);
  pq.Push(940328);
  pq.Push(29);
  pq.Push(93);
  pq.Push(50);

  EXPECT_EQ(pq.Size(), 8);
  EXPECT_EQ(pq.Top(), 28);

  pq.Pop();
  EXPECT_EQ(pq.Top(), 28);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 29);
  pq.Push(3);
  EXPECT_EQ(pq.Size(), 7);
  EXPECT_EQ(pq.Top(), 3);
}

TEST(PQueue, less) {
  PQueue<int> pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);

  EXPECT_EQ(pq.Top(), 2);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 23);
}

TEST(PQueue, great) {
  PQueue<int, std::greater<int>> pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);

  EXPECT_EQ(pq.Top(), 42);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 34);
}

class MyClass {
 public:
  explicit MyClass(int n) : n_(n) {}
  bool operator<(const MyClass& mc) const { return n_ < mc.n_; }
  int n() { return n_; }

 private:
  int n_;
};

class CompareMyClassLess {
 public:
  bool operator()(MyClass* kObj1, MyClass* kObj2) {
    return kObj1->n() < kObj2->n();
  }
};

class CompareMyClassGreater {
 public:
  bool operator()(MyClass* kObj1, MyClass* kObj2) {
    return kObj1->n() > kObj2->n();
  }
};

TEST(PQueue, custom_class) {
  std::vector<MyClass> vec{MyClass(42), MyClass(23), MyClass(2), MyClass(34)};

  PQueue<MyClass> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top().n(), vec[2].n());
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top().n(), vec[1].n());
}

TEST(PQueue, custom_class_pointer_less) {
  std::vector<MyClass*> vec{new MyClass(42), new MyClass(23), new MyClass(2),
                            new MyClass(34)};

  PQueue<MyClass*, CompareMyClassLess> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top(), vec[2]);
  EXPECT_EQ(pq.Size(), 4);
  delete pq.Top();
  pq.Pop();
  EXPECT_EQ(pq.Top(), vec[1]);

  for (int i = 0; i < 3; i++) {
    delete pq.Top();
    pq.Pop();
  }
}

TEST(PQueue, custom_class_pointer_greater) {
  std::vector<MyClass*> vec{new MyClass(42), new MyClass(23), new MyClass(2),
                            new MyClass(34)};

  PQueue<MyClass*, CompareMyClassGreater> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top(), vec[0]);
  EXPECT_EQ(pq.Size(), 4);
  delete pq.Top();
  pq.Pop();
  EXPECT_EQ(pq.Top(), vec[3]);

  for (int i = 0; i < 3; i++) {
    delete pq.Top();
    pq.Pop();
  }
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
