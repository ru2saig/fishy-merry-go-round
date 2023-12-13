CXX=g++
CXXFLAGS=-std=c++20 -pedantic -Wall -Iinclude
LDFLAGS=-lraylib -lpthread -ldl

all: main.out

main.out: main.o FishManager.o Fish.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm *.o main.out

.PHONY: clean all
