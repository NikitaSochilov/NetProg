CXX = g++
CXXFLAGS = -Wall

all: daytime

daytime: daytime.o
	$(CXX) $(CXXFLAGS) -o daytime daytime.o

daytime.o: daytime.cpp
	$(CXX) $(CXXFLAGS) -c daytime.cpp

clean:
	rm -f *.o daytime
