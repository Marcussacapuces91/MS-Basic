CXX=g++
CXXFLAGS=-g -Wall -Werror -std=c++11 -Iinclude
LDLIBS=

PRGM = MSBasic
# SRCS := $(wildcard *.cpp)
# OBJS := $(SRCS:.cpp=.o)
# DEPS := $(OBJS:.o=.d)

all: $(PRGM)
	echo "for ALL, need $$(PRGM)"

$(PRGM): main.o
	echo "for $$(PRGM), need main.o"

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

