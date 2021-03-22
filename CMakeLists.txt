cmake_minimum_required(VERSION 3.15)

project(c2amqp-adaptor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src)

include_directories(${PROJECT_SOURCE_DIR}/third_party)

# add_subdirectory("third_party/pistache")

# add_subdirectory("third_party/rapidjson")
