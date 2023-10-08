# Linux Chat

<div align="center">
<a href="https://github.com/Livings5858/Chat/">简体中文</a>
&emsp;&emsp; | &emsp;&emsp;
English
</div> 
<br>

## Dependencies

* Google Test
    
    The Linux Chat project relies on Google Test. You can follow the detailed build instructions at [Google Test's README](https://github.com/google/googletest/blob/main/googletest/README.md).

## Building

```bash
make
```

The results will be generated in the `out` directory:

| File                  | Function                                                  |
|-----------------------|-----------------------------------------------------------|
| server                | Linux Chat server program                                |
| client                | Linux Chat client program                                |
| test_chat_message     | Test program for serializing and deserializing message protocols |
| test_tcp_connect      | Test program for testing TCP connections, message sending, and handling |

## File Structure

| Directory/File                    | Function                                                  |
|-----------------------------------|-----------------------------------------------------------|
| client.cpp                        | Main program for Linux Chat client                        |
| common/chat_message.cpp           | Source code for implementing serialization and deserialization of Linux Chat message protocols |
| common/chat_message.h             | Header file for Linux Chat message protocols              |
| connect/tcp_client.cpp            | Source code for implementing TCP connections in Linux Chat client |
| connect/tcp_client.h              | Header file for TCP connections in Linux Chat client       |
| connect/tcp_server.cpp            | Source code for implementing TCP connections in Linux Chat server |
| connect/tcp_server.h              | Header file for TCP connections in Linux Chat server       |
| makefile                          | Makefile for building the project                         |
| message_handling/message_handler.cpp | Source code for Linux Chat message handling              |
| message_handling/message_handler.h   | Header file for Linux Chat message handling              |
| server.cpp                        | Main program for Linux Chat server                        |
| test/test_chat_message.cpp        | Test source code for serializing and deserializing Linux Chat message protocols |
| test/test_tcp_connect.cpp          | Test source code for Linux Chat TCP connections           |