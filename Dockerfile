FROM ubuntu:22.04

# Install dependency libraries
RUN apt-get update -y
RUN apt-get install -y cmake ninja-build clang git openssl zlib1g libmongoc-dev python3.9 python-is-python3 libasio-dev

RUN git clone --recursive https://github.com/tadashibashi/insound-cpp /app

WORKDIR /app

# Copy dependencies to system lib directory
COPY ./lib/fmod/lib/linux/libfmod.so.13 /usr/lib/libfmod.so.13
COPY ./lib/fsbank/lib/linux/libfsbank.so.13 /usr/lib/libfsbank.so.13
COPY ./lib/fsbank/lib/linux/libfsbvorbis.so /usr/lib/libfsbvorbis.so
COPY ./lib/fsbank/lib/linux/libopus.so /usr/lib/libopus.so.0

# Build project
WORKDIR /app/build
WORKDIR /app
RUN cmake -G Ninja -S . -B build
RUN cmake --build build --target insound-server

CMD ./build/insound-server
