cmake_minimum_required(VERSION 3.21)

project(QtProcess VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Network
    Protobuf
    Grpc
)

qt_standard_project_setup()

# Path to your proto file
set(PROTO_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../proto/echo/v1/echo.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/../proto/logger/v1/logger.proto
)

qt_add_protobuf(proto_lib
    GENERATE_PACKAGE_SUBFOLDERS
    PROTO_FILES ${PROTO_FILES}
)

qt_add_grpc(proto_grpc CLIENT
    GENERATE_PACKAGE_SUBFOLDERS
    PROTO_FILES ${PROTO_FILES}
)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        loggerclient.cpp
        loggerclient.h
        echoclient.cpp
        echoclient.h
        grpcunarycall.h
)

qt_add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Protobuf
    Qt${QT_VERSION_MAJOR}::Grpc
    proto_lib
    proto_grpc
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
