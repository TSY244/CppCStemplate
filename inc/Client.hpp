//
// Created by 12414 on 2023-07-23.
//

#ifndef CPPINTERNALEXAM_CLIENT_HPP
#define CPPINTERNALEXAM_CLIENT_HPP
#include "iostream"
#include"unistd.h"
#include "arpa/inet.h"//socket
#include "ConsoleLog.hpp"
#include <iostream>
#include <cstring>
#include "RecvAndSend.hpp"


#define  PORT 8998

class Client {
private:
    int clientFd;
    TinyLog::ConsoleLog logger;
public:
    Client();
    ~Client();
    void worker();
};


#endif //CPPINTERNALEXAM_CLIENT_HPP
