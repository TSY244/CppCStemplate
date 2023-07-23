//
// Created by 12414 on 2023-07-23.
//

#ifndef CPPINTERNALEXAM_RECVANDSEND_HPP
#define CPPINTERNALEXAM_RECVANDSEND_HPP

#include <cstdint>
#include <unistd.h>
#include "sys/socket.h"
namespace std {
    bool safeRecive(int fd, char *buf, uint32_t n, int flags);

    bool safeSend(int fd, char *buf, ::uint32_t n, int flags);

} // std

#endif //CPPINTERNALEXAM_RECVANDSEND_HPP
