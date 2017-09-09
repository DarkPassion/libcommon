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

file(GLOB STRING_SRCS src/string/*.cpp)
file(GLOB STRING_INCS src/string/*.h)

file(GLOB DIGEST_SRCS src/digest/*.cpp)
file(GLOB DIGEST_INCS src/digest/*.h)

file(GLOB THREAD_SRCS src/thread/*.cpp)
file(GLOB THREAD_INCS src/thread/*.h)

file(GLOB UTIL_SRCS src/util/*.cpp)
file(GLOB UTIL_INCS src/util/*.h src/util/*.hpp)

file(GLOB QUEUE_SRCS src/queue/*.cpp)
file(GLOB QUEUE_INCS src/queue/*.h)

file(GLOB TIMER_SRCS src/timer/*.cpp)
file(GLOB TIMER_INCS src/timer/*.h)

file(GLOB TASK_SRCS src/task/*.cpp)
file(GLOB TASK_INCS src/task/*.h)

file(GLOB NETWORK_SRCS src/network/*.cpp)
file(GLOB NETWORK_INCS src/network/*.h)

file(GLOB_RECURSE JSON_CPP_SRCS ${PROJECT_SOURCE_DIR}/3rd/jsoncpp/*.cpp)
file(GLOB_RECURSE JSON_CPP_INCS ${PROJECT_SOURCE_DIR}/3rd/jsoncpp/*.h)

file(GLOB_RECURSE SOCKET_SERVER_SRCS ${PROJECT_SOURCE_DIR}/3rd/socket_server/*.c)
file(GLOB_RECURSE SOCKET_SERVER_INCS ${PROJECT_SOURCE_DIR}/3rd/socket_server/*.h)

set (JSON_CPP_XX_FILES
    ${JSON_CPP_SRCS}
    ${JSON_CPP_INCS}
)

set (SOCKET_SERVER_XX_FILES
    ${SOCKET_SERVER_SRCS}
    ${SOCKET_SERVER_INCS}
)

source_group("strings\\Source Files" FILES ${STRING_SRCS})
source_group("strings\\Header Files" FILES ${STRING_INCS})

source_group("digets\\Source Files" FILES ${DIGEST_SRCS})
source_group("digets\\Header Files" FILES ${DIGEST_INCS})

source_group("thread\\Source Files" FILES ${THREAD_SRCS})
source_group("thread\\Header Files" FILES ${THREAD_INCS})

source_group("util\\Source Files" FILES ${UTIL_SRCS})
source_group("util\\Header Files" FILES ${UTIL_INCS})

source_group("queue\\Source Files" FILES ${QUEUE_SRCS})
source_group("queue\\Header Files" FILES ${QUEUE_INCS})

source_group("timer\\Source Files" FILES ${TIMER_SRCS})
source_group("timer\\Header Files" FILES ${TIMER_INCS})

source_group("task\\Source Files" FILES ${TASK_SRCS})
source_group("task\\Header Files" FILES ${TASK_INCS})

source_group("network\\Source Files" FILES ${NETWORK_SRCS})
source_group("network\\Header Files" FILES ${NETWORK_INCS})

source_group("3rd\\socket_server\\" FILES ${SOCKET_SERVER_XX_FILES})
source_group("3rd\\jsoncpp" FILES ${JSON_CPP_XX_FILES})


set (LIB_SRCS
    ${STRING_SRCS}
    ${DIGEST_SRCS}
    ${THREAD_SRCS}
    ${UTIL_SRCS}
    ${QUEUE_SRCS}
    ${TIMER_SRCS}
    ${TASK_SRCS}
    ${NETWORK_SRCS}
    ${JSON_CPP_SRCS}
    ${SOCKET_SERVER_SRCS}
)

set(LIB_HEADERS
    ${STRING_INCS}
    ${DIGEST_INCS}
    ${THREAD_INCS}
    ${UTIL_INCS}
    ${QUEUE_INCS}
    ${TIMER_INCS}
    ${TASK_INCS}
    ${NETWORK_INCS}
    ${JSON_CPP_INCS}
    ${SOCKET_SERVER_INCS}
)


add_library(common${MYVER} STATIC ${LIB_SRCS} ${LIB_HEADERS})
#add_library(common${MYVER} SHARED ${LIB_SRCS} ${LIB_HEADERS})




