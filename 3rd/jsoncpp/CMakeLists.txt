cmake_minimum_required(VERSION 2.8)

PROJECT(jsoncpp${MYVER})
set (CMAKE_BUILD_TYPE DEBUG)

set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/)

link_directories(${PROJECT_BINARY_DIR}/bin/ /usr/local/lib/)
include_directories( ${PROJECT_SOURCE_DIR}/.
${PROJECT_SOURCE_DIR}/src
${PROJECT_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "-D_CONSOLE -g -DUSE_GNUTLS -Wall -O2 -D_CONSOLE -pipe -Wno-invalid-offsetof -fcheck-new -D_POSIX_THREADS -D_POSIX_THREAD_SAFE_FUNCTIONS -D_REENTRANT")

set (LIB_SRC 
src/json_reader.cpp
src/json_value.cpp
src/json_writer.cpp)


add_library(jsoncpp${MYVER}
${LIB_SRC})





