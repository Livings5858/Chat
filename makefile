CXX = g++
CXXFLAGS = -std=c++14 -Wall

all: client server test

client: client.cpp
	$(CXX) $(CXXFLAGS) -o out/clinet client.cpp

server: server.cpp
	$(CXX) $(CXXFLAGS) -o out/server server.cpp

test: test.o
	$(CXX) $(CXXFLAGS) -o out/test out/test.o -lgtest -lgtest_main -pthread

test.o: test/test.cpp
	$(CXX) $(CXXFLAGS) -Icommon -c test/test.cpp -o out/test.o

clean:
	rm out/*

.PHONY: all clean
