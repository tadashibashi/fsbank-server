# [Insound Server](https://github.com/tadashibashi/insound-cpp)

Server powering Insound, the music and sound hosting service for professional
audio presentations (in development)

Gives composers and sound designers an approachable and
customizable way to present interactive audio to their clients

For the frontend repository please visit
[here](https://github.com/tadashibashi/insound-frontend)

## Technologies Used

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![AWS](https://img.shields.io/badge/AWS-%23FF9900.svg?style=for-the-badge&logo=amazon-aws&logoColor=white)
![MongoDB](https://img.shields.io/badge/MongoDB-%234ea94b.svg?style=for-the-badge&logo=mongodb&logoColor=white)
![JWT](https://img.shields.io/badge/JWT-black?style=for-the-badge&logo=JSON%20web%20tokens)
![FMOD](https://img.shields.io/badge/FMOD-212121?style=for-the-badge)


## Getting Started

Eventually a public link will be provided at which the website will be hosted.
For now, you will need to build the project locally.

When cloning this repository, please make sure to recursively initialize
its submodules via
`git clone --recursive https://github.com/tadashibashi/insound-cpp` or if it is
already cloned, run `git submodule update --init --recursive` in the project
root.


#### Supported Platforms

Currently, only Linux Ubuntu 23 (mantic) amd64 and macOS 14 arm (M1) are tested
and supported.

Supporting Windows for development may be considered, if necessary.


#### System dependencies

Pease make sure the following are available on your system before building:

- C++ 20 compiler (tested with clang 16)
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

Some Linux package distributors do not have recent enough versions of
libmongoc, libcurl, or aws-sdk-cpp, etc. so you may need to compile and install
them from source.

For a comprehensive installation command list, please check the Dockerfile in
the repository root, which demonstrates a complete installation on Ubuntu Linux
23 (mantic) amd64.


#### External dependencies

- MongoDB storage access with read/write rights
- S3 storage key with read/write admin rights over all buckets
- Nodemailer-compatible email service that implements receiving the basic
    JSON options in the [message config](https://nodemailer.com/message/)

#### Server Environment Variables

| Name                  | Description                                         |
| --------------------- | --------------------------------------------------- |
| JWT_SECRET            | String signing for JSON web tokens                  |
| PASSWORD_SECRET       | String for hashing passwords                        |
| PASSWORD_SALT         | String for salting password hashes                  |
| MONGO_URL             | MongoDB url, including password & port number       |
| MONGO_DBNAME          | Name of server's mongo database                     |
| AWS_ENDPOINT_URL      | AWS Endpoint URL, including port if applicable      |
| AWS_ACCESS_KEY_ID     | AWS Access ID string                                |
| AWS_SECRET_ACCESS_KEY | AWS Secret Key string                               |
| S3_BUCKET             | Name of the server's S3 bucket                      |
| EMAIL_ENDJPOINT_URL   | Endpoint for a nodemailer-compatible smtp service   |
| EMAIL_ACCESS_KEY      | Access key for smtp service                         |
| EMAIL_AUTOMATED_SENDER| Address of the site automated email sender          |

For local builds, you may create a .env file in the root of this
repo, which will automatically load and populate the environment.
(All .env are gitignored)

#### Build

Via Python script helper (Python 3.9+ should be installed):
```shell
# In the root directory of this repo:

# Grant executable permissions to python script file
chmod +x run

# Configure, build & run server
./run serve

# Configure, build & run tests
./run test
```

Without helper script:
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

To build the frontend:

- [Bun](https://bun.sh/docs/installation) should be installed

```shell
# In a working directory where you want to create the frontend repo, call:
git clone https://github.com/tadashibashi/insound-frontend

cd insound-frontend
bun install
bun run dev
```

If the script ran successfully, a dev server has started at which you may
visit the frontend at http://localhost:5173, or where the terminal message
indicates.


#### Running Tests

The server test suite provides and populates its own envionment variables, so
regarding env variables no setup on your part is necessary. However, you will
need:
- MongoDB server running on your machine at `mongodb://localhost:27017`
- AWS S3-compatible storage server running on your machine at
`http://localhost:9000`

### Docker Deployment

Granted that the external dependencies listed above are available and the
environment variables are set accordingly, deploying via docker can be
done via

```shell
    docker build . --platform=linux/amd64
```


## Credits

### Included as submodules

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

## Roadmap

- [x] Email API
    - [x] Send emails

- [ ] User API
    - [ ] Authorization on private routes
    - [ ] Create Account (email confirmation with activation token)
    - [ ] Read Profile Page (private / public settings)
    - [ ] Patch profile info
    - [ ] Delete Account

- [ ] Track API
    - [ ] Uploads include converted fsb
    - [ ] Download raw audio as ZIP
    - [ ] Patch
        - [ ] New audio tracks
        - [ ] Replace existing audio
        - [ ] Remove existing audio
        - [ ] All patches replace with new fsb
    - [ ] Delete
        - [ ] Ensure cascading deletion of all files on S3 + fsb

- [ ] Test suites for all server routes

- Future:
    - Admin routing
    - Windows dev support, if necessary
