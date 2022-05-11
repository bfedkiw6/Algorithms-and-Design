targets = 

CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11

all: $(targets)

clean:
	rm -f $(targets)
