cmake_minimum_required(VERSION 3.12)
project(project_service CXX)

include(cmake/Config.cmake)
include(third_party/userver/cmake/SetupEnvironment.cmake)
include(GNUInstallDirs)

add_subdirectory(third_party/userver)

# Common sources
add_library(
  ${PROJECT_NAME}_objs OBJECT
  src/project_service.hpp
  src/project_service.cpp
  src/auth_client.hpp
  src/auth_client.cpp
  src/middlewares/jwt_auth_middleware.hpp
  src/middlewares/jwt_auth_middleware.cpp
  src/middlewares/auth_middleware_component.hpp
  src/middlewares/auth_middleware_component.cpp)

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${ELECTRONIC_PATH}/api ${PROJECT_SOURCE_DIR}/proto/api)

# Grpc enable
include(GrpcTargets)
add_grpc_library(
  ${PROJECT_NAME}_proto PROTOS api/project_service/v1/project_service.proto
  api/auth_service/v1/auth_service.proto)

target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver-postgresql
                                                  ${PROJECT_NAME}_proto)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

# Functional Tests
include(UserverTestsuite)

set(testsuite_requirements "${CMAKE_CURRENT_SOURCE_DIR}/tests/requirements.txt")
if(Protobuf_VERSION VERSION_GREATER 3.20.0)
  list(APPEND testsuite_requirements
       "${USERVER_DIR}/testsuite/requirements-grpc.txt")
else()
  list(APPEND testsuite_requirements
       "${USERVER_DIR}/testsuite/requirements-grpc-old.txt")
  message(STATUS "Forcing old protobuf version for testsuite")
endif()

userver_testsuite_add_simple(REQUIREMENTS ${testsuite_requirements})

# Install
if(DEFINED ENV{PREFIX})
  message(STATUS "Set install prefix: $ENV{PREFIX}")
  file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
  set(CMAKE_INSTALL_PREFIX "${PREFIX_PATH}")
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml
     ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT ${PROJECT_NAME})
install(
  FILES ${CONFIGS_FILES}
  DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}
  COMPONENT ${PROJECT_NAME})
