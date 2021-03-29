#!/bin/bash
# script to run the demo

DEMO_DIR='./src/'

cd ${DEMO_DIR}
BROKER_BIN='echo ./broker'
RECEIVER_BIN='echo ./simple_recv'
ADAPTER_BIN='echo ./ceph-amqp1.0-adaptor'

tmux new-session -d -s demo
tmux split-window -h
tmux send-keys "${BROKER_BIN}" "C-m"
tmux split-window -v
tmux send-keys "${RECEIVER_BIN}" "C-m"
tmux split-window -v
tmux send-keys "${ADAPTER_BIN}" "C-m"
tmux attach-session -t demo
