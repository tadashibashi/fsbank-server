FROM ubuntu:22.04

ENV APP_DIR=/app
ENV BUILD_TYPE=release

# Install dependency libraries
RUN apt-get update -y && apt-get install -y --no-install-recommends \
    clang \
    cmake \
    curl \
    git \
    libmongoc-dev \
    libasio-dev \
    libcurl4-openssl-dev \
    lld \
    ninja-build \
    openssl \
    python3.9 \
    python-is-python3 \
    zlib1g

# Clone repo
# Invalidate cache every commit
ADD https://api.github.com/repos/tadashibashi/insound-cpp/git/refs/heads/main \
    /version.json
RUN git config --global http.sslverify false && \
    git clone --recursive https://github.com/tadashibashi/insound-cpp $APP_DIR

WORKDIR $APP_DIR

# Build project & clean up
RUN chmod +x run && ./run build $BUILD_TYPE && \
    mv ./lib/fmod/lib/linux/libfmod.so.13 /usr/lib/libfmod.so.13 && \
    mv ./lib/fsbank/lib/linux/libfsbank.so.13 /usr/lib/libfsbank.so.13 && \
    mv ./lib/fsbank/lib/linux/libfsbvorbis.so /usr/lib/libfsbvorbis.so && \
    mv ./lib/fsbank/lib/linux/libopus.so /usr/lib/libopus.so.0 && \
    apt-get remove -y python3.9 python-is-python3 git clang ninja-build cmake \
    lld libcurl4-openssl-dev && \
    apt-get clean autoclean && \
    apt-get autoremove -y && \
    rm -rf /var/lib/{apt,dpkg,cache,log}/ ./lib ./insound ./tests ./main.cpp \
        ./.git/ ./.gitignore ./.gitmodules ./Dockerfile ./CMakeLists.txt

CMD ./build/$BUILD_TYPE/insound-server
