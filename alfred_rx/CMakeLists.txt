cmake_minimum_required(VERSION 2.8.12)
project(AlfredRX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(alfredRX_HDRS inc/alfredRX.h
inc/alfred.h
inc/client.h)
set(alfredRX_SRCS src/alfredRX.cpp
src/client.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/)
add_executable(AlfredRX ${alfredRX_HDRS} ${alfredRX_SRCS})
# <------------ add hiredis dependency here --------------->
find_path(HIREDIS_HEADER hiredis)
target_include_directories(AlfredRX PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
target_link_libraries(AlfredRX ${HIREDIS_LIB})


# <------------ add redis-plus-plus dependency -------------->
# NOTE: this should be *sw* NOT *redis++*
find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(AlfredRX PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(AlfredRX ${REDIS_PLUS_PLUS_LIB})

target_link_libraries(AlfredRX pthread)
