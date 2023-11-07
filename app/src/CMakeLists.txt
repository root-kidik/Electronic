set(CPP_SRC
    main.cpp
    main_window.cpp
    main_window.hpp
    services/auth_service.hpp
    services/auth_service.cpp
    widgets/registration_widget.hpp
    widgets/registration_widget.cpp
    widgets/login_widget.hpp
    widgets/login_widget.cpp
    widgets/profile_widget.hpp
    widgets/profile_widget.cpp)

set(PROTO_ROOT ../../api)
set(PROTO_SRC ${PROTO_ROOT}/auth_service/v1/auth_service.proto)

find_package(Qt6 REQUIRED COMPONENTS Widgets Protobuf Grpc)
qt_add_executable(${PROJECT_NAME} ${CPP_SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Protobuf
                                              Qt6::Grpc)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)

qt_add_protobuf(${PROJECT_NAME} PROTO_FILES ${PROTO_SRC})
qt_add_grpc(${PROJECT_NAME} CLIENT PROTO_FILES ${PROTO_SRC})
