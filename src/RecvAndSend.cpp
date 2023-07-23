//
// Created by 12414 on 2023-07-23.
//


#include "../inc/RecvAndSend.hpp"

namespace std {
    bool safeRecive(int fd, char *buf, uint32_t n, int flags) {
        /*
         * description: 更安全的recv
         * return：判断成功
         */
        uint32_t count = 0;
        uint32_t i;
        while ((i = recv(fd, buf + count, n - count, flags)) > 0) {
            count += i;
            if (count >= n) {
                return true;
            }
        }
        return false;
    }

    bool safeSend(int fd, char *buf, ::uint32_t n, int flags) {
        uint32_t count = 0; //已发送的数据字节数
        uint32_t i;
        while ((i = send(fd, buf + count, n - count, flags)) > 0) {
            count += i;
            if (count >= n) {
                return true;
            }
        }
        return false;
    }
} // std