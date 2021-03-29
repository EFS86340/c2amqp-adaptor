# The Dockerfile for demo adapter
#
FROM ubuntu:20.04

# dependencies for qpid-proton
RUN apt-get update && apt-get install -y gcc g++ cmake cmake-curses-gui uuid-dev \
	libssl-dev libsasl2-2 libsasl2-dev libsasl2-modules swig python-dev

RUN apt-get install -y tmux git ruby-dev doxygen vim curl

WORKDIR "/c2amqp"

COPY . /c2amqp/

RUN cd /
# RUN git clone https://github.com/apache/qpid-proton.git
RUN cd third_party/qpid-proton && mkdir build && cd build && cmake .. \
	-DCMAKE_INSTALL_PREFIX=/usr -DSYSINSTALL_BINDINGS=ON && make && make install

RUN cd /
# RUN git clone https://github.com/pistacheio/pistache.git
RUN cd third_party/pistache && mkdir build && cd build && cmake .. && make && make install

RUN cd /c2amqp/ && mkdir build && cd build && cmake .. && make
CMD bash demo_run.sh
