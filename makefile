CXX = g++
CXXFLAGS = -std=c++14 -Wall

all: client server test

client: client.cpp tcp_client.o
	$(CXX) $(CXXFLAGS) -o out/clinet client.cpp out/tcp_client.o -Iconnect

server: server.cpp tcp_server.o
	$(CXX) $(CXXFLAGS) -o out/server server.cpp out/tcp_server.o -Iconnect

test: test.o
	$(CXX) $(CXXFLAGS) -o out/test out/test.o -lgtest -lgtest_main -pthread

test.o: test/test.cpp
	$(CXX) $(CXXFLAGS) -Icommon -c test/test.cpp -o out/test.o

tcp_server.o: connect/tcp_server.cpp
	$(CXX) $(CXXFLAGS) -Icommon -c connect/tcp_server.cpp -o out/tcp_server.o

tcp_client.o: connect/tcp_client.cpp
	$(CXX) $(CXXFLAGS) -Icommon -c connect/tcp_client.cpp -o out/tcp_client.o

clean:
	rm out/*

.PHONY: all clean
