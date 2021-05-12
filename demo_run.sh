#!/bin/bash
# script to run the demo

DEMO_DIR='/c2amqp/build/src/'
DESTINATIONS=("127.0.0.1:5672/examples" "127.0.0.1:5673/wangxu")

cd ${DEMO_DIR}
BROKER_BIN='./demo_broker'
RECEIVER_BIN='./demo_simple_recv'
ADAPTER_BIN='./ceph-amqp1.0-adaptor'

tmux new-session -d -s demo
tmux split-window -h
tmux send-keys "${BROKER_BIN} -a ${DESTINATIONS[0]}" "C-m"
tmux split-window -v
tmux send-keys "${BROKER_BIN} -a ${DESTINATIONS[1]}" "C-m"
tmux split-window -v
tmux send-keys "${RECEIVER_BIN} -a ${DESTINATIONS[0]}" "C-m"
tmux split-window -v
tmux send-keys "${RECEIVER_BIN} -a ${DESTINATIONS[1]}" "C-m"
tmux split-window -v
tmux send-keys "${ADAPTER_BIN}" "C-m"
tmux attach-session -t demo
