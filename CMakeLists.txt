cmake_minimum_required(VERSION 3.22)
project(CppInternalExam)

set(CMAKE_CXX_STANDARD 20)

add_executable(CppInternalExam2 ClientMain.cpp inc/Client.hpp src/Client.cpp src/RecvAndSend.cpp inc/RecvAndSend.hpp)

add_executable(CppInternalExam main.cpp src/Server.cpp inc/Server.hpp inc/ThreadPool.h inc/ConsoleLog.hpp src/RecvAndSend.cpp inc/RecvAndSend.hpp)


target_link_libraries(CppInternalExam  pthread)