//
// Created by 12414 on 2023-07-23.
//

#include <cstring>
#include "../inc/Client.hpp"

Client::Client() {
    //Use the logging library
    logger.setName("logger");
    logger.setELevel(TinyLog::ConsoleLog::ELevel::eDebug);


    clientFd = socket(AF_INET, SOCK_STREAM, 0);

    int ret;
    sockaddr_in clientAddr{0};
    clientAddr.sin_port = htons(PORT);
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int cliAddrLen = sizeof(clientAddr);
    ret = connect(clientFd, reinterpret_cast<sockaddr *>(&clientAddr), static_cast<socklen_t>(cliAddrLen));
    if (ret == -1) {
        LOG_ERROR(logger, "client connect error")
        exit(-1);
    }
}

Client::~Client() {
    close(clientFd);
}

void Client::worker() {
    while (true) {
        char sendMeg[BUFSIZ] = {0};
        std::cin>>sendMeg;
        if (!std::safeSend(clientFd, sendMeg, sizeof(sendMeg), 0)) {
            LOG_WARN(logger, "client send error")
        }
        memset(sendMeg,0,sizeof (sendMeg));

        char recvMeg[BUFSIZ]{0};
        if (!std::safeRecive(clientFd, recvMeg, sizeof(recvMeg), 0)) {
            LOG_WARN(logger, "client recv error")
        }
        std::cout << recvMeg << std::endl;
        memset(recvMeg, 0, sizeof(recvMeg));
    }

}
