# The Dockerfile for demo adapter
#
FROM ubuntu:20.04

# dependencies for qpid-proton
RUN apt-get update && apt-get install -y gcc g++ cmake gdb libspdlog-dev

RUN apt-get install -y tmux git ruby-dev doxygen vim curl \
	libqpid-proton-cpp12-dev

# install pistache-dev from ppa
RUN apt-get update && apt-get install -y software-properties-common --fix-missing && add-apt-repository ppa:pistache+team/unstable
RUN apt-get update && apt-get install -y libpistache-dev

WORKDIR "/c2amqp"

COPY . /c2amqp/

# RUN cd /
# # RUN git clone https://github.com/apache/qpid-proton.git
# RUN cd third_party/qpid-proton && mkdir build && cd build && cmake .. \
# 	-DCMAKE_INSTALL_PREFIX=/usr -DSYSINSTALL_BINDINGS=ON && make && make install
# 
# RUN cd /
# RUN git clone https://github.com/pistacheio/pistache.git
# RUN cd pistache && mkdir build && cd build && cmake .. && make && make install

RUN cd /c2amqp/ && mkdir build && cd build && cmake .. && make
CMD bash demo_run.sh
