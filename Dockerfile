FROM ubuntu:22.04

# Install dependency libraries
RUN apt-get update -y && apt-get install -y cmake ninja-build clang git \
    openssl zlib1g libmongoc-dev python3.9 python-is-python3 libasio-dev lld

ENV APP_DIR=/app
ENV BUILD_TYPE=release

# Clone repo
# Invalidate cache every commit
ADD https://api.github.com/repos/tadashibashi/insound-cpp/git/refs/heads/main \
    /version.json
RUN git clone --recursive https://github.com/tadashibashi/insound-cpp $APP_DIR

WORKDIR $APP_DIR

# Copy dependencies to system lib directory
COPY ./lib/fmod/lib/linux/libfmod.so.13 /usr/lib/libfmod.so.13
COPY ./lib/fsbank/lib/linux/libfsbank.so.13 /usr/lib/libfsbank.so.13
COPY ./lib/fsbank/lib/linux/libfsbvorbis.so /usr/lib/libfsbvorbis.so
COPY ./lib/fsbank/lib/linux/libopus.so /usr/lib/libopus.so.0

# Build project
RUN python3 run build $BUILD_TYPE

# Clean up build tools & source
RUN apt-get remove -y python3.9 python-is-python3 git clang ninja-build cmake \
    lld && rm -rf ./lib ./insound ./tests ./main.cpp ./.git/ \
    ./.gitignore ./.gitmodules ./Dockerfile \
    ./CMakeLists.txt && apt-get autoremove -y

CMD ./build/$BUILD_TYPE/insound-server
