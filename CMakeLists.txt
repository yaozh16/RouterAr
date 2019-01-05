cmake_minimum_required(VERSION 3.5)
project(routeAr)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3 ")
find_package(Threads REQUIRED)





add_executable(V2 V2.c analyseip.h analyseip.c arpfind.h checksum.h recvroute.h sendetherip.h lookuproute.h lookuproute.c recvroute.c analyseip.c checksum.c arpfind.c sendetherip.c routeTableSTree.h routeTableSTree.c)


if(THREADS_HAVE_PTHREAD_ARG)
    set_property(TARGET V2 PROPERTY COMPILE_OPTIONS "-pthread")
    set_property(TARGET V2 PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()

target_link_libraries(V2 "${CMAKE_THREAD_LIBS_INIT}")


#add_executable(TestRoute testRoute.c routeTableSTree.c routeTableSTree.h)
#if(THREADS_HAVE_PTHREAD_ARG)
#    set_property(TARGET TestRoute PROPERTY COMPILE_OPTIONS "-pthread")
#    set_property(TARGET TestRoute PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
#endif()

#target_link_libraries(TestRoute "${CMAKE_THREAD_LIBS_INIT}")