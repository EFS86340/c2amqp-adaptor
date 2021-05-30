#! /bin/bash
#

set -ex 

BROKER_URL="https://github.com/EFS86340/c2amqp-adaptor/releases/download/v3.1/demo_broker"
RECV_URL="https://github.com/EFS86340/c2amqp-adaptor/releases/download/v3.1/demo_simple_recv"

TOPIC_DEST="localhost:5672/amqp1_0"

BROKER_BIN="amqp-broker"
RECEIVER_BIN="amqp-receiver"

curl -Lj0 ${BROKER_URL} -o ${BROKER_BIN}
curl -Lj0 ${RECV_URL} -o ${RECEIVER_BIN}

chmod u+x ${BROKER_BIN}
chmod u+x ${RECEIVER_BIN}

# skip md5 check

# run the broker
tmux new-session -d -s amqp-test
tmux split-window -h
tmux send-keys "./${BROKER_BIN} -a ${TOPIC_DEST}" "C-m"
tmux split-window -v
tmux send-keys "./${RECEIVER_BIN} -a ${TOPIC_DEST}" "C-m"
tmux attach-session -t amqp-test
