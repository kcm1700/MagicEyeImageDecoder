TARGETS += main
OBJS += main.o
CXX := g++
CXXFLAGS += -O3 -std=c++0x
CXXFLAGS += -Wall
CXXFLAGS += -Werror=implicit-function-declaration -Werror=implicit-int

all: $(TARGETS)

main: main.o
	$(CXX) $(CXXFLAGS) -o $@ $<

main.o: main.cpp types.h exceptions.h MagicImage.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGETS) $(OBJS)

