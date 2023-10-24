FROM ubuntu:22.04

ENV APP_DIR=/app
ENV BUILD_TYPE=release
WORKDIR $APP_DIR

# Invalidate cache every commit
ADD https://api.github.com/repos/tadashibashi/insound-cpp/git/refs/heads/main \
    /version.json

# Install dependency libraries
RUN apt-get update -y && apt-get install -y --no-install-recommends \
    clang \
    cmake \
    curl \
    git \
    libmongoc-1.0-0 \
    libasio-dev \
    libcurl4-openssl-dev \
    lld \
    ninja-build \
    libssl-dev \
    python3.9 \
    python-is-python3 \
    zlib1g && \
    git config --global http.sslverify false && \
    git clone --recursive https://github.com/tadashibashi/insound-cpp $APP_DIR \
    && chmod +x run && ./run install $BUILD_TYPE insound-server "/usr/" && \
    apt-get remove -y python3.9 python-is-python3 git clang ninja-build \
        cmake lld && \
    apt-get clean autoclean && \
    apt-get autoremove -y && \
    for dir in $APP_DIR/*; do [ "$dir" = "public" ] && continue rm -rf "$dir"; done &&\
    rm -rf /var/lib/{apt,dpkg,cache,log}/ $APP_DIR

CMD insound-server
