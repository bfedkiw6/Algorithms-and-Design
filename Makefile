targets = zap unzap

CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11

all: $(targets)

zap: zap.cc huffman.h pqueue.h bstream.h
	$(CXX) $(CXXFLAGS) -o $@ $<

unzap: unzap.cc huffman.h pqueue.h bstream.h
	$(CXX) $(CXXFLAGS) -o $@ $<

# The rules below are just for our googletesting purposes
test_pqueue: test_pqueue.cc pqueue.h
	$(CXX) $(CXXFLAGS) -o $@ $< -lgtest -lpthread

test_bstream: test_bstream.cc bstream.h
	$(CXX) $(CXXFLAGS) -o $@ $< -lgtest -lpthread

lint:
	~/Programs/C++_Code/cpplint *.cc *.h

clean:
	rm -f $(targets) test_pqueue test_bstream *.zap *.unzap
