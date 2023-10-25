FROM ubuntu:22.04

ENV APP_DIR=/app
ENV BUILD_TYPE=release

# Invalidate cache every commit
ADD https://api.github.com/repos/tadashibashi/insound-cpp/git/refs/heads/main \
    /version.json

# Install dependencies, build, clean up
RUN \
    # Install dependencies
        apt-get update -y && apt-get install -y --no-install-recommends \
        # Build tools
            clang \
            cmake \
            git \
            python3.9 \
            python-is-python3 \
            lld \
            ninja-build \
        # Runtime libraries
            curl \
            libasio-dev \
            libcurl4-openssl-dev \
            libssl-dev \
            zlib1g-dev && \
    # Build, install & clean-up mongo-c-driver (until package updated to v1.24)
        git config --global http.sslverify false && \
        git clone https://github.com/mongodb/mongo-c-driver.git && \
        mkdir -p mongo-c-driver/cmake-build && \
        cd mongo-c-driver/cmake-build && \
        cmake -DCMAKE_INSTALL_PREFIX=/usr/ -DCMAKE_C_COMPILER=clang \
            -DCMAKE_PREFIX_PATH=.. -G Ninja .. && \
        cmake --build . && \
        cmake --install . && \
        cd ../.. && \
        rm -rf mongo-c-driver && \
    # Build, install & clean-up aws-sdk-cpp (until package available in apt)
        git clone --recursive https://github.com/aws/aws-sdk-cpp && \
        mkdir -p aws-sdk-cpp/cmake-build && \
        cd aws-sdk-cpp/cmake-build && \
        cmake -DCMAKE_INSTALL_PREFIX=/usr/ -DCMAKE_C_COMPILER=clang \
            -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release \
            -DBUILD_ONLY="s3" .. && \
        cmake --build . --config=Release && \
        cmake --install . --config=Release && \
        cd ../.. && rm -rf aws-sdk-cpp && \
    # Build, install & clean-up server
        git clone --recursive https://github.com/tadashibashi/insound-cpp $APP_DIR && \
        cd $APP_DIR && \
        chmod +x run && ./run install $BUILD_TYPE insound-server "/usr/" && \
        rm -rf $APP_DIR && \
    # Clean up build tools
        apt-get remove -y python3.9 python-is-python3 git clang ninja-build \
            cmake lld && \
        apt-get clean autoclean && \
        apt-get autoremove -y && \
        rm -rf /var/lib/{apt,dpkg,cache,log}/

CMD insound-server
