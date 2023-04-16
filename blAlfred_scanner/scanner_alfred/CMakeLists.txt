cmake_minimum_required(VERSION 2.8.12)

find_package(PkgConfig REQUIRED)

pkg_search_module(GATTLIB REQUIRED gattlib)
pkg_search_module(PCRE REQUIRED libpcre)

set(ble_scanner_HDRS inc/arraylist.h 
inc/ble_scanner.h 
inc/packet.h 
inc/ble_scanner.h 
inc/deque.h 
inc/hashtable.h 
inc/alfred.h 
inc/client.h)

set(ble_scanner_SRCS src/ble_scanner.c 
src/arraylist.c 
src/deque.c 
src/hashtable.c 
src/client.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/)
add_executable(ble_scanner ${ble_scanner_HDRS} ${ble_scanner_SRCS})
target_link_libraries(ble_scanner ${GATTLIB_LIBRARIES} ${GATTLIB_LDFLAGS} ${PCRE_LIBRARIES} pthread)
