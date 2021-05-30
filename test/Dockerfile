# The Dockerfile for a quick brokers and receivers setup.
# 
FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

# dependencies for qpid-proton
RUN apt-get update && apt-get install -y gcc g++ cmake curl git tmux ruby-dev \
	doxygen libqpid-proton-cpp12-dev

WORKDIR /amqp-test

COPY . /amqp-test

# here we expose 2 ports for our brokers
EXPOSE 5672 5673

RUN cd /amqp-test 
CMD bash setup_test.sh
