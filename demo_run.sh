#!/bin/bash
# script to run the demo

DEMO_DIR='/c2amqp/build/src/'

cd ${DEMO_DIR}
BROKER_BIN='./demo_broker'
RECEIVER_BIN='./demo_simple_recv'
ADAPTER_BIN='./ceph-amqp1.0-adaptor'

tmux new-session -d -s demo
tmux split-window -h
tmux send-keys "${BROKER_BIN}" "C-m"
tmux split-window -v
tmux send-keys "${RECEIVER_BIN}" "C-m"
tmux split-window -v
tmux send-keys "${ADAPTER_BIN}" "C-m"
tmux attach-session -t demo
