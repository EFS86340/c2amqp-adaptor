# aux_source_directory(. SRC)

set(ADAPTER_INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(ADAPTER_HEADER ${ADAPTER_INCLUDE}/adaptor.h ${ADAPTER_INCLUDE}/qpid_proton.h
		${ADAPTER_INCLUDE}/ts_queue.h)
set(ADAPTER_SRC main.cc adaptor.cc qpid_proton.cc)

set(QPID_EXAMPLE_HEADER ${ADAPTER_INCLUDE}/options.hpp)

set(ADAPTER ceph-amqp1.0-adaptor)

# set(pistache_DIR
# 		${CMAKE_SOURCE_DIR}/third_party/pistache/src/PistacheConfig.cmake.in)
find_package(pistache)
# set(proton_DIR
# 		${CMAKE_SOURCE_DIR}/third_party/qpid_proton/cpp/ProtonCppConfig.cmake.in)
find_package(proton)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

add_executable(${ADAPTER} ${ADAPTER_HEADER} ${ADAPTER_SRC})

# target_include_directories(${ADAPTER} PRIVATE ${ADAPTER_INCLUDE})
target_link_libraries(${ADAPTER} PRIVATE pistache pthread)
target_link_libraries(${ADAPTER} PRIVATE qpid-proton-cpp)

add_executable(demo_broker ${QPID_EXAMPLE_HEADER} broker.cpp)
target_link_libraries(demo_broker PRIVATE qpid-proton-cpp)
add_executable(demo_simple_recv ${QPID_EXAMPLE_HEADER} simple_recv.cpp)
target_link_libraries(demo_simple_recv PRIVATE qpid-proton-cpp)

target_link_libraries(${ADAPTER} PRIVATE spdlog::spdlog)
