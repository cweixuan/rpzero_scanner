cmake_minimum_required(VERSION 2.8.12)

find_package(PkgConfig REQUIRED)

pkg_search_module(GATTLIB REQUIRED gattlib)
pkg_search_module(PCRE REQUIRED libpcre)

set(ble_scanner_HDRS ble_scanner.h)
set(ble_scanner_SRCS ble_scanner.c)

add_executable(ble_scanner ${ble_scanner_SRCS} ${ble_scanner_SRCS})
target_link_libraries(ble_scan ${GATTLIB_LIBRARIES} ${GATTLIB_LDFLAGS} ${PCRE_LIBRARIES} pthread)
