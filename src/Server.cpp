//
// Created by 12414 on 2023-07-23.
//

#include "../inc/Server.hpp"

Server::Server() {
    //Use the logging library
    logger.setName("logger");
    logger.setELevel(TinyLog::ConsoleLog::ELevel::eDebug);//use debug level

    // connect using ipv4
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {
        LOG_ERROR(logger, "Client socket initialization error")
        exit(-1);
    }

    auto option{1};//启用端口复用
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, (const char *) (&option), sizeof(option))) {
        LOG_WARN(logger, "Failed to start port multiplexing")
        close(serverFd);
        //exit(-1);
    }

    //record address
    sockaddr_in addr{};
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    addr.sin_family = AF_INET;

    int ret;
    ret = bind(serverFd, (sockaddr *) &addr, sizeof(addr));
    if (ret == -1) {
        LOG_ERROR(logger, "server bind error")
        close(serverFd);
        exit(-1);
    }


    ret = listen(serverFd, 10);
    if (ret == -1) {
        LOG_ERROR(logger, "server listen error")
        close(serverFd);
        exit(-1);
    }
}

Server::~Server() {
    close(serverFd);
}


int Server::manager() {
    while (true) {
        ClientInfo clientInfo;
        int cliAddrLen{sizeof(*clientInfo.clientAddr)};
        clientInfo.clientFd = accept(serverFd,
                                     reinterpret_cast<sockaddr *>(clientInfo.clientAddr),
                                     reinterpret_cast<socklen_t *>(&cliAddrLen));
        if (clientInfo.clientFd == -1) {
            LOG_WARN(logger, "accept error")
            exit(-1);
        }
        threadPool.commit([clientInfo, this]() {
            while (true) {
                char recvMeg[BUFSIZ]{0};
                if (!std::safeRecive(clientInfo.clientFd, recvMeg, sizeof(recvMeg), 0)) {
                    LOG_WARN(logger, "server recv error")
                }
                std::cout << recvMeg << std::endl;
                memset(recvMeg, 0, sizeof(recvMeg));


                char sendMeg[BUFSIZ]{"this is server"};
                if (!std::safeSend(clientInfo.clientFd, sendMeg, sizeof(sendMeg), 0)) {
                    LOG_WARN(logger, "server send error")
                }
                memset(sendMeg, 0, sizeof(recvMeg));
            }

        });
    }

}

