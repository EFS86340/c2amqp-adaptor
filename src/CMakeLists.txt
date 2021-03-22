aux_source_directory(. SRC)

add_executable(ceph-amqp1.0-adaptor ${SRC})

target_link_libraries(ceph-amqp1.0-adaptor PRIVATE pistache pthread)
