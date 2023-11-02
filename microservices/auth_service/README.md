# 🔐 Authservice 
Microservice for registration, authentication 

## 👾 Os 
- MacOS
- Linux

## ⚙️ Configure
- Install [CMake](https://cmake.org)
- Install [PostgreSQL](https://www.postgresql.org)
- Install [Python](https://www.python.org)
- Configure [userver](https://userver.tech/d3/da9/md_en_2userver_2tutorial_2build.html)

## 🔨 Build and Run 🚀
* `make build-debug` - debug build of the service with all the assertions and sanitizers enabled
* `make build-release` - release build of the service with LTO
* `make test-debug` - does a `make build-debug` and runs all the tests on the result
* `make test-release` - does a `make build-release` and runs all the tests on the result
* `make service-start-debug` - builds the service in debug mode and starts it
* `make service-start-release` - builds the service in release mode and starts it
* `make format` - autoformat all the C++ and Python sources
* `make clean-` - cleans the object files
* `make dist-clean` - clean all, including the CMake cached configurations
* `make install` - does a `make build-release` and runs install in directory set in environment `PREFIX`
* `make install-debug` - does a `make build-debug` and runs install in directory set in environment `PREFIX`

## 🧿 Api
- [Grpc](../../api)

## ⚒️ Tools
- Build generator: [CMake](https://cmake.org)
- Code formatter: [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
- Code static analyzer: [clang-tidy](https://clang.llvm.org/extra/clang-tidy)

## 🗒️ Database
- [PostgreSQL](https://www.postgresql.org)

## 📚 Third party
- Microservice framework: [userver](https://userver.tech/d3/da9/md_en_2userver_2tutorial_2build.html)
- Testing microservice framework: [Testsuite](https://github.com/yandex/yandex-taxi-testsuite)

