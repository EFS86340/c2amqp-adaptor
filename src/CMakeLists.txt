# aux_source_directory(. SRC)

set(ADAPTER_INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(ADAPTER_HEADER ${ADAPTER_INCLUDE}/adaptor.h ${ADAPTER_INCLUDE}/qpid_proton.h
		${ADAPTER_INCLUDE}/ts_queue.h)
set(ADAPTER_SRC main.cc adaptor.cc qpid_proton.cc)

set(QPID_EXAMPLE_HEADER ${ADAPTER_INCLUDE}/options.hpp)

set(ADAPTER ceph-amqp1.0-adaptor)

add_executable(${ADAPTER} ${ADAPTER_HEADER} ${ADAPTER_SRC})

# target_include_directories(${ADAPTER} PRIVATE ${ADAPTER_INCLUDE})
target_link_libraries(${ADAPTER} PRIVATE pistache pthread)
target_link_libraries(${ADAPTER} PRIVATE qpid-proton-cpp)

add_executable(broker ${QPID_EXAMPLE_HEADER} broker.cpp)
target_link_libraries(broker PRIVATE qpid-proton-cpp)
add_executable(simple_recv ${QPID_EXAMPLE_HEADER} simple_recv.cpp)
target_link_libraries(simple_recv PRIVATE qpid-proton-cpp)
