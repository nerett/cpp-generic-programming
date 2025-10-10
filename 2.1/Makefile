CXX = clang++
CXXFLAGS = -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -g -MMD -MP

TARGET = cow_test
SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)
DEPS = $(OBJECTS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJECTS) $(DEPS)

run: $(TARGET)
	./$(TARGET)

-include $(DEPS)
