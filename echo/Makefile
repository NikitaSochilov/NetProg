CXX = g++
CXXFLAGS = -Wall

all: echo_client

echo_client: echo_client.o
	$(CXX) $(CXXFLAGS) -o echo_client echo_client.o

echo_client.o: echo_client.cpp
	$(CXX) $(CXXFLAGS) -c echo_client.cpp

clean:
	rm -f *.o echo_client
