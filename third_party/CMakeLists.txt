cmake_minimum_required(VERSION 3.15)

project(adaptor_third_party)

set(PISTACHE_BUILD_TESTS OFF)
set(PISTACHE_BUILD_EXAMPLES OFF)
set(PISTACHE_BUILD_DOCS OFF)

add_subdirectory(pistache)
add_subdirectory(qpid-proton)
