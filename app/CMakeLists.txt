cmake_minimum_required(VERSION 3.27.7)

include(cmake/Config.cmake)

project(Electronic)

add_subdirectory(third_party/jwt-cpp)
add_subdirectory(src)
