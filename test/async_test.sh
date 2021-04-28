#!/bin/bash
#
# this script aims to simply mock a bunch of HTTP notitications from the Ceph
# Rados Gateway(RGW).

set -ex

# we send multiple notifications to different AMQP1.0 endpoints
# we should have setup the broker and the receiver
DESTINATIONS=("127.0.0.1:5672/examples" "127.0.0.1:5673/wangxu")

BROKER_BIN='../build/src/demo_broker'
RECV_BIN='../build/src/demo_simple_recv'

setup_broker() {
	for dest in ${DESTINATIONS[@]}
	do
		echo "[info] broker - ${dest}"
		${BROKER_BIN} -a ${dest} >> broker.out&
	done
}

setup_receiver() {
	for dest in ${DESTINATIONS[@]}
	do
		echo "[info] receiver - ${dest}"
		${RECV_BIN} -a ${dest} >> recv.out&
	done
}


# function to send the notifications
notify() {
	local msg=$1
	curl -d "${msg}" -H "Content-Type:application/json" -X POST http://localhost:7074
}

main() {
	setup_broker
	sleep 1
	setup_receiver
	sleep 5

	for i in `seq 1 10`
	do
		notify "{\"key1\":\"127.0.0.1:5672/examples\", \"$(date)\":\"value${i}\"}" &
		notify "{\"key1\":\"127.0.0.1:5673/wangxu\", \"$(date)\":\"value${i}\"}" &
	done

	sleep 5
}

trap 'jobs -p | xargs -r kill' EXIT
# main()
main $@
