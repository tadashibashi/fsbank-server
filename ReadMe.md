# Insound

## External Dependencies
- C++ 20 compiler (currently tested with clang 16)
- cmake

### External Libraries
- aws-sdk-cpp
- ca-certificates
- curl + libcurl
- libasio
- libmongoc 1.24+
- libssl
- zlib

### Included Submodules
- bcrypt
- catch2
- crow
- glaze
- jwt-cpp
- mongo-cxx-driver
- spdlog
- zip


Optional, for python build scripts
- ninja
- python 3.9+
- psutil (python library for killing processes, optional)

Please check Dockerfile for explicit installation commands for
Ubuntu Linux amd64

TODO: Windows compatibility. CMakeLists files need to be updated to move dll's
to binary dir, amongst other unknown compatibility issues.
