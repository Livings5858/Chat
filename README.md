# Linux Chat

<div align="center">
简体中文
&emsp;&emsp; | &emsp;&emsp;
<a href="https://github.com/Livings5858/Chat/blob/main/README_en.md">English</a>
</div> 
<br>


## 依赖

* Google Test

    Linux Chat 项目依赖于 Google Test。你可以按照 [Google Test 的README文档](https://github.com/google/googletest/blob/main/googletest/README.md) 获取详细构建说明。

## 构建

```bash
make
```

结果将生成在out目录下：

| 文件                 | 功能                                                     |
|----------------------|----------------------------------------------------------|
| server               | Linux Chat 的服务端程序                                   |
| client               | Linux Chat 的客户端程序                                   |
| test_chat_message    | 测试程序，测试消息协议的序列化和反序列化                      |
| test_tcp_connect     | 测试程序，测试 TCP 连接、消息的发送和处理                      |

## 文件结构

| 目录/文件                         | 功能                                       |
|----------------------------------|--------------------------------------------|
| client.cpp                       | Linux Chat 的客户端主程序                   |
| common/chat_message.cpp          | 实现 Linux Chat 消息协议的序列化和反序列化的源代码文件 |
| common/chat_message.h            | Linux Chat 消息协议的头文件                 |
| connect/tcp_client.cpp           | Linux Chat 客户端的 TCP 连接实现的源代码文件    |
| connect/tcp_client.h             | Linux Chat 客户端的 TCP 连接的头文件          |
| connect/tcp_server.cpp           | Linux Chat 服务端的 TCP 连接实现的源代码文件    |
| connect/tcp_server.h             | Linux Chat 服务端的 TCP 连接的头文件          |
| makefile                         | 用于构建项目的 Makefile 文件                |
| message_handling/message_handler.cpp | Linux Chat 消息处理的源代码文件            |
| message_handling/message_handler.h   | Linux Chat 消息处理的头文件                |
| server.cpp                       | Linux Chat 的服务端主程序                   |
| test/test_chat_message.cpp       | Linux Chat 消息协议的序列化和反序列化的测试源代码文件 |
| test/test_tcp_connect.cpp         | Linux Chat TCP 连接的测试源代码文件          |