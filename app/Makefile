CMAKE_COMMON_FLAGS ?= -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
NPROCS ?= $(shell nproc)
CLANG_FORMAT ?= clang-format

# Debug cmake configuration
build_debug/Makefile:
	@mkdir -p build_debug
	@cd build_debug && \
      cmake -DCMAKE_BUILD_TYPE=Debug $(CMAKE_COMMON_FLAGS) ..

# Release cmake configuration
build_release/Makefile:
	@mkdir -p build_release
	@cd build_release && \
      cmake -DCMAKE_BUILD_TYPE=Release $(CMAKE_COMMON_FLAGS) ..

# Run cmake
.PHONY: cmake-debug cmake-release
cmake-debug cmake-release: cmake-%: build_%/Makefile

# Build using cmake
.PHONY: build-debug build-release
build-debug build-release: build-%: cmake-%
	@cmake --build build_$* -j $(NPROCS) --target Electronic 

# Start the service (via testsuite service runner)
.PHONY: app-start-debug app-start-release
app-start-debug app-start-release: app-start-%: build-%
	@cd ./build_$* && $(MAKE) Electronic && ./bin/Electronic

# Cleanup data
.PHONY: clean-debug clean-release
clean-debug clean-release: clean-%:
	cd build_$* && $(MAKE) clean

.PHONY: dist-clean
dist-clean:
	@rm -rf build_*

# Format the sources
.PHONY: format
format:
	@find src -name '*pp' -type f | xargs $(CLANG_FORMAT) -i
