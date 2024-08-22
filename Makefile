all: $(PRGM)

$(PRGM): $(OBJS)
    $(CXX) $(OBJS) $(LDLIBS) -o $@

%.o: %.cpp
    $(CXX) $(CXXFLAGS) -MMD -MP -c £< -o $@

clean:
    rm -rf $(OBJS) $(DEPS) $(PRGM)
