FROM ubuntu:bionic

RUN apt-get update && apt-get install -y build-essential cmake libsfml-dev fonts-open-sans

ENV DISPLAY $DISPLAY

COPY /highway /highway

WORKDIR /highway 

RUN rm -rf build && mkdir build && cd build && cmake .. && cmake --build .
