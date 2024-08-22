CXX=g++
CXXFLAGS=-g -Wall -Werror -std=c++11
INCLUDE=-I./include
LDLIBS=
OBJD=./obj
SRCD=./src

obj/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@

.PHONY: clean
clean:
	rm -f *.o
