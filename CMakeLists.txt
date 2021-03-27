cmake_minimum_required(VERSION 3.15)

project(c2amqp-adaptor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
add_subdirectory(src)

add_subdirectory("third_party")

set(pistache_DIR
		${CMAKE_SOURCE_DIR}/third_party/pistache/src/PistacheConfig.cmake.in)
find_package(pistache)
set(proton_DIR
		${CMAKE_SOURCE_DIR}/third_party/qpid_proton/cpp/ProtonCppConfig.cmake.in)
find_package(proton)
