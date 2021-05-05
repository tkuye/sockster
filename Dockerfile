FROM ubuntu

RUN apt get update

ENV execute sockster
# Install lib event library 
 
RUN mkdir build
RUN cd build

RUN cmake ..
RUN make 

RUN ${execute}



