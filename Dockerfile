FROM ubuntu:22.04

ENV appdir=/app1

# Install dependency libraries
RUN apt-get update -y && apt-get install -y cmake ninja-build clang git \
    openssl zlib1g libmongoc-dev python3.9 python-is-python3 libasio-dev

# Clone repo
RUN git clone --recursive https://github.com/tadashibashi/insound-cpp $appdir

WORKDIR $appdir

# Copy dependencies to system lib directory
COPY ./lib/fmod/lib/linux/libfmod.so.13 /usr/lib/libfmod.so.13
COPY ./lib/fsbank/lib/linux/libfsbank.so.13 /usr/lib/libfsbank.so.13
COPY ./lib/fsbank/lib/linux/libfsbvorbis.so /usr/lib/libfsbvorbis.so
COPY ./lib/fsbank/lib/linux/libopus.so /usr/lib/libopus.so.0

# Build project
WORKDIR $appdir

# Config and build
RUN python3 run build RelWithDebInfo

# Clean up build tools
RUN apt-get remove -y python3.9 python-is-python3 git clang ninja-build cmake

# Clean up source
RUN rm -rf /app/lib $appdir/tests $appdir/main.cpp $appdir/.git/ $appdir/.gitignore \
    $appdir/.gitmodules $appdir/Dockerfile $appdir/CMakeLists.txt

CMD ./build/relwithdebinfo/insound-server
