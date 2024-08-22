CXX=g++          
CXXFLAGS=-g -Wall -Werror -std=c++11
LDLIBS=

#PRGM = MSBasic
#SRCS := $(wildcard *.cpp)
#OBJS := $(SRCS:.cpp=.o)
#DEPS := $(OBJS:.o=.d)

# all: $(PRGM)

# $(PRGM): $(OBJS)
#	$(CXX) $(OBJS) $(LDLIBS) -o $@

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) src/$< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJS) $(DEPS) $(PRGM)
