CXX = g++
CXXFLAGS = -std=c++14 -Wall

all: create_out_directory client server test_chat_message test_tcp_connect

create_out_directory:
	mkdir -p out

client: client.cpp tcp_client.o chat_message.o
	$(CXX) $(CXXFLAGS) -o out/clinet client.cpp out/chat_message.o out/tcp_client.o -Iconnect -Icommon

server: server.cpp tcp_server.o chat_message.o
	$(CXX) $(CXXFLAGS) -o out/server server.cpp out/chat_message.o out/tcp_server.o -Iconnect -Icommon

test_chat_message: test_chat_message.o chat_message.o
	$(CXX) $(CXXFLAGS) -o out/test_chat_message out/test_chat_message.o out/chat_message.o -lgtest -lgtest_main -pthread

test_tcp_connect: test_tcp_connect.o tcp_server.o tcp_client.o chat_message.o message_handler.o
	$(CXX) $(CXXFLAGS) -o out/test_tcp_connect \
	out/test_tcp_connect.o \
	out/tcp_server.o \
	out/tcp_client.o \
	out/chat_message.o \
	out/message_handler.o \
	-lgtest -lgtest_main -pthread

test_chat_message.o: test/test_chat_message.cpp
	$(CXX) $(CXXFLAGS) -Icommon -c test/test_chat_message.cpp -o out/test_chat_message.o

test_tcp_connect.o: test/test_tcp_connect.cpp
	$(CXX) $(CXXFLAGS) \
	-Icommon \
	-Iconnect \
	-Imessage_handling \
	-c \
	test/test_tcp_connect.cpp \
	-o out/test_tcp_connect.o

tcp_server.o: connect/tcp_server.cpp
	$(CXX) $(CXXFLAGS) -Icommon -c connect/tcp_server.cpp -o out/tcp_server.o -pthread

tcp_client.o: connect/tcp_client.cpp
	$(CXX) $(CXXFLAGS) -Icommon -c connect/tcp_client.cpp -o out/tcp_client.o

chat_message.o: common/chat_message.cpp
	$(CXX) $(CXXFLAGS) -Icommon -c common/chat_message.cpp -o out/chat_message.o

message_handler.o: message_handling/message_handler.cpp
	$(CXX) $(CXXFLAGS) -Icommon -c message_handling/message_handler.cpp -o out/message_handler.o

clean:
	rm out/*

.PHONY: all create_out_directory clean test_chat_message test_tcp_connect
