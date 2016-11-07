cmake_minimum_required(VERSION 2.8)


PROJECT(LZ4${MYVER})



set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs/)


file(GLOB LIB_SRC_C  *.c)
file(GLOB LIB_SRC_CXX  *.cpp)

add_library(LZ4${MYVER}
    ${LIB_SRC_C}
    ${LIB_SRC_CXX})


