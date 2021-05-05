FROM ubuntu


RUN apt get update

# Install lib event library 
RUN git clone https://github.com/libevent/libevent.git

RUN cd libevent
RUN mkdir build && cd build
RUN cmake ..
RUN make
RUN make verify 
RUN make install 


RUN cd ../../build

RUN cmake ..
RUN make 




