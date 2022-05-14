targets = zap unzap

CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11

all: $(targets)

# Finish the Makefile as we go because I'm not sure what the dependencies are yet
zap: zap.cc huffman.h
	$(CXX) $(CXXFLAGS) -o $@ $<

unzap: unzap.cc huffman.h
	$(CXX) $(CXXFLAGS) -o $@ $<

# The rules below are just for our googletesting purposes
test_pqueue: test_pqueue.cc pqueue.h
	$(CXX) $(CXXFLAGS) -o $@ $< -lgtest -lpthread

test_bstream: test_bstream.cc bstream.h
	$(CXX) $(CXXFLAGS) -o $@ $< -lgtest -lpthread

clean:
	rm -f $(targets)
