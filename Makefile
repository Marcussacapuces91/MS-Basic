CXX=g++          # The C++ compiler
CXXFLAGS=-g -Wall -Werror -std=c++1      # C++ complilation flags
LDLIBS=

PRGM = MSBasic
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

all: $(PRGM)

$(PRGM): $(OBJS)
	$(CXX) $(OBJS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c £< -o $@

clean:
	rm -rf $(OBJS) $(DEPS) $(PRGM)
