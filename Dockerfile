FROM gcc:11.1

RUN apt-get update && apt-get install -y build-essential libsfml-dev fonts-open-sans

COPY /highway /simproj

WORKDIR /highway 

RUN mkdir build && cd build && cmake .. && cmake --build .
