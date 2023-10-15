CXX = g++
CXXFLAGS = -std=c++14 -Wall

all: create_out_directory client server test_chat_message test_tcp_connect test_log

create_out_directory:
	mkdir -p out

client: client.cpp tcp_client.o chat_message.o client_message_handler.o
	$(CXX) $(CXXFLAGS) -o out/clinet \
	client.cpp \
	out/chat_message.o \
	out/tcp_client.o \
	out/client_message_handler.o \
	-Iconnect \
	-Icommon

server: server.cpp tcp_server.o chat_message.o server_message_handler.o logger.o
	$(CXX) $(CXXFLAGS) -o out/server \
	server.cpp \
	out/chat_message.o \
	out/tcp_server.o \
	out/server_message_handler.o \
	out/logger.o \
	-Iconnect \
	-Icommon \
	-Imessage_handling

test_chat_message: test_chat_message.o chat_message.o
	$(CXX) $(CXXFLAGS) -o out/test_chat_message out/test_chat_message.o out/chat_message.o -lgtest -lgtest_main -pthread

test_tcp_connect: test_tcp_connect.o tcp_server.o tcp_client.o \
	chat_message.o server_message_handler.o client_message_handler.o logger.o
	$(CXX) $(CXXFLAGS) -o out/test_tcp_connect \
	out/test_tcp_connect.o \
	out/tcp_server.o \
	out/tcp_client.o \
	out/chat_message.o \
	out/server_message_handler.o \
	out/client_message_handler.o \
	out/logger.o \
	-lgtest -lgtest_main -pthread

test_log: test/test_log.cpp logger.o
	$(CXX) $(CXXFLAGS) -o out/test_log \
	-Iutils \
	test/test_log.cpp \
	out/logger.o \
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
	$(CXX) $(CXXFLAGS) \
	-Icommon \
	-Imessage_handling \
	-Iutils \
	-c connect/tcp_server.cpp -o out/tcp_server.o -pthread

tcp_client.o: connect/tcp_client.cpp
	$(CXX) $(CXXFLAGS) \
	-Icommon \
	-Imessage_handling \
	-c connect/tcp_client.cpp -o out/tcp_client.o -pthread

chat_message.o: common/chat_message.cpp
	$(CXX) $(CXXFLAGS) -Icommon -c common/chat_message.cpp -o out/chat_message.o

server_message_handler.o: message_handling/server_message_handler.cpp
	$(CXX) $(CXXFLAGS) \
	-Icommon \
	-Imessage_handling \
	-Iutils \
	-c message_handling/server_message_handler.cpp \
	-o out/server_message_handler.o

client_message_handler.o: message_handling/client_message_handler.cpp
	$(CXX) $(CXXFLAGS) \
	-Icommon \
	-Imessage_handling \
	-c message_handling/client_message_handler.cpp \
	-o out/client_message_handler.o

logger.o : utils/logger.cpp
	$(CXX) $(CXXFLAGS) \
	-Iutils \
	-c utils/logger.cpp \
	-o out/logger.o

clean:
	rm out/*

.PHONY: all create_out_directory clean test_chat_message test_tcp_connect test_log
