# Insound

## External Dependencies
- C++ 20 compiler (tested with clang only)

Libraries
- aws-sdk-cpp
- cmake 3.24+
- curl + libcurl
- libasio
- libmongoc 1.24+
- libssl
- zlib

- readable ca certificates to make ssl requests

Optional, for python build scripts
- ninja
- python 3.9+
- psutil (python library for killing processes, optional)

Please check Dockerfile for explicit installation commands for
Ubuntu Linux amd64

TODO: Windows compatibility. CMakeLists files need to be updated to move dll's
to binary dir, amongst other unknown compatibility issues.
