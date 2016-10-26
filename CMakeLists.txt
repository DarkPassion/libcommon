cmake_minimum_required(VERSION 2.8)

PROJECT(common${MYVER})
set (CMAKE_BUILD_TYPE DEBUG)

set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs/)

link_directories(${PROJECT_BINARY_DIR}/bin/ /usr/local/lib/ ${PROJECT_BINARY_DIR}/3rd/jsoncpp/lib)
link_libraries(common${MYVER} pthread jsoncpp)
include_directories( ${PROJECT_SOURCE_DIR}/.
${PROJECT_SOURCE_DIR}/src
${PROJECT_SOURCE_DIR}/3rd/jsoncpp/include)

set(CMAKE_CXX_FLAGS "-DWEBRTC_POSIX -g -DUSE_GNUTLS -Wall -O2 -D_CONSOLE -pipe -Wno-invalid-offsetof -fcheck-new -D_REENTRANT")

file(GLOB STRING src/string/*.cpp)
file(GLOB DIGEST src/digest/*.cpp)
file(GLOB THREAD src/thread/*.cpp)
file(GLOB UTIL src/util/*.cpp)
file(GLOB QUEUE src/queue/*.cpp)
file(GLOB TIMER src/timer/*.cpp)
file(GLOB TASK src/task/*.cpp)
file(GLOB NETWORK src/network/*.cpp)

set (LIB_SRC
    ${STRING}
    ${DIGEST}
    ${THREAD}
    ${UTIL}
    ${QUEUE}
    ${TIMER}
    ${TASK}
    ${NETWORK}
)


add_library(common${MYVER}
${LIB_SRC})





