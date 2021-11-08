FROM ubuntu:bionic

RUN apt-get update && apt-get install -y build-essential cmake libsfml-dev=2.4.2+dfsg-4 fonts-open-sans=1.11-1
RUN apt-cache policy libsfml-dev fonts-open-sans
ENV DISPLAY $DISPLAY

COPY /highway /highway

WORKDIR /highway 

RUN rm -rf build && mkdir build && cd build && cmake .. && cmake --build .
