
cmake_minimum_required(VERSION 3.15)

project(QRCodeGen)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE LIBS "QR-Code-generator/cpp/qrcodegen.*")

add_library(${PROJECT_NAME} STATIC
    ${LIBS}
)

target_include_directories(${PROJECT_NAME} INTERFACE
    # $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
)
