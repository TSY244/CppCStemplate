//
// Created by 12414 on 2023-07-23.
//

#ifndef CPPINTERNALEXAM_SERVER_HPP
#define CPPINTERNALEXAM_SERVER_HPP


/**
 * 只负责连接，和调用工作的函数
 */

#include "iostream"
#include"unistd.h"
#include "arpa/inet.h"//socket
#include "ConsoleLog.hpp"
#include <iostream>
#include "cstring"
#include "ThreadPool.h"
#include "RecvAndSend.hpp"


#define PORT 8998

struct ClientInfo{
    int clientFd;
    sockaddr_in* clientAddr;
    ClientInfo():clientFd(-1), clientAddr(nullptr){}
};

class Server {
private:
    int serverFd;
    TinyLog::ConsoleLog logger;
    std::ThreadPool threadPool;
public:
    int manager();
public:
    Server();
    ~Server();
};


#endif //CPPINTERNALEXAM_SERVER_HPP
