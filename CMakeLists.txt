cmake_minimum_required(VERSION 3.5)
project(routeAr)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")



find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    set_property(TARGET my_app PROPERTY COMPILE_OPTIONS "-pthread")
    set_property(TARGET my_app PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()






add_executable(V2 V2.c analyseip.h analyseip.c arpfind.h checksum.h recvroute.h sendetherip.h lookuproute.h lookuproute.c recvroute.c analyseip.c checksum.c arpfind.c sendetherip.c routeTableSTree.h routeTableSTree.c)
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(V2 "${CMAKE_THREAD_LIBS_INIT}")
endif()


add_executable(TestRoute testRoute.c routeTableSTree.c routeTableSTree.h)
