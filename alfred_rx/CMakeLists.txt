cmake_minimum_required(VERSION 2.8.12)
project(AlfredRX)

set(alfredRX_HDRS inc/alfredRX.h)
set(alfredRX_SRCS src/alfredRX.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/)
add_executable(alfredRX ${alfredRX_HDRS} ${alfredRX_SRCS})
target_link_libraries(alfredRX pthread)
