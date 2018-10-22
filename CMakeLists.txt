cmake_minimum_required(VERSION 3.5)
project(routeAr)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")






add_executable(Test main.c analyseip.h arpfind.h checksum.h recvroute.h sendetherip.h lookuproute.h lookuproute.c)
