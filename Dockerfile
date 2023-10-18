FROM ubuntu:22.04

# Install dependency libraries
RUN apt-get update -y
RUN apt-get install -y cmake g++ openssl zlib1g libmongoc-dev

COPY . /app

WORKDIR /app

# Copy dependencies to system lib directory
COPY ./lib/fmod/lib/linux/libfmod.so.13 /usr/lib/libfmod.so.13
COPY ./lib/fsbank/lib/linux/libfsbank.so.13 /usr/lib/libfsbank.so.13
COPY ./lib/fsbank/lib/linux/libfsbvorbis.so /usr/lib/libfsbvorbis.so
COPY ./lib/fsbank/lib/linux/libopus.so /usr/lib/libopus.so

# Build project
WORKDIR /app/build
WORKDIR /app
RUN cmake -S . -B build
RUN cmake --build build

# Install built executable to system bin directory
RUN mv build/fsbank-server /usr/bin/fsbank-server

# Clean up
RUN rm -rf /app

# Run server
CMD ["fsbank-server"]
