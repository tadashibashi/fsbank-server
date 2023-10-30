# Insound

A music and sound hosting service for professional audio presentations
(in development)

Gives composers and sound designers an approachable and
customizable way to present interactive audio to their clients

## Technologies Used

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![AWS](https://img.shields.io/badge/AWS-%23FF9900.svg?style=for-the-badge&logo=amazon-aws&logoColor=white)
![JWT](https://img.shields.io/badge/JWT-black?style=for-the-badge&logo=JSON%20web%20tokens)
![MongoDB](https://img.shields.io/badge/MongoDB-%234ea94b.svg?style=for-the-badge&logo=mongodb&logoColor=white)
![FMOD](https://img.shields.io/badge/FMOD-212121?style=for-the-badge)
![Python](https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54)

## Getting Started

### Contributing Development

Before attempting to build the project, please make sure the following
are available on your system.

#### System dependencies
- C++ 20 compiler (tested with clang 14-16)
- cmake 3.24+
- aws-sdk-cpp
- ca-certificates
- curl + libcurl
- libasio
- libmongoc 1.24+
- libssl
- zlib


Optional, for python build scripts
- ninja
- python 3.9+
- psutil (optional)

Some Linux package distributors do not have recent enough versions of libmongoc
or aws-sdk-cpp so you may need to compile and install them from source.

For a comprehensive installation command list, please check Dockerfile, which
demonstrates a complete installation on Ubuntu Linux amd64.

#### Build

Currently, only Linux amd64 and Mac arm are tested and supported.

Build via Python script helper (Python 3.9+ should be installed)
```shell
# In the root directory of this repo:

# Grant executable permissions to python script file
chmod +x run

# Configure, build & run server
./run serve

# Configure, build & run tests
./run test
```

Build without helper script:
```shell
# In the root directory of this repo:

# Configure build
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

# Build & run server
cmake --build build --config Debug --target insound-server
build/insound/server/insound-server

# Build & run test executable
cmake --build build --config Debug --target insound-tests
build/insound/tests/insound-tests
```


TODO: Windows compatibility. CMakeLists files need to be updated to move dll's
to binary dir, amongst other unknown compatibility issues.

### Credits

#### Included as submodules

| Library                                                         | License   |
| --------------------------------------------------------------- | --------- |
| [bcrypt](https://github.com/hilch/Bcrypt.cpp)                   | BSD 4     |
| [catch2](https://github.com/catchorg/Catch2)                    | Boost     |
| [crow](https://github.com/CrowCpp/Crow)                         | BSD 3     |
| [glaze](https://github.com/stephenberry/glaze)                  | MIT       |
| [jwt-cpp](https://github.com/Thalhammer/jwt-cpp)                | MIT       |
| [mongo-cxx-driver](https://github.com/mongodb/mongo-cxx-driver) | Apache    |
| [spdlog](https://github.com/gabime/spdlog)                      | MIT       |
| [zip](https://github.com/kuba--/zip)                            | Unlicense |
