cmake_minimum_required(VERSION 3.15)

project(IpaInstallation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PNG REQUIRED)

file(GLOB LIBS "lib/*.cpp")

add_library(${PROJECT_NAME} STATIC
    ${LIBS}
)

if(PNG_FOUND)
    message("libpng found ${PNG_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "libpng not found")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/include
    ${PNG_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} INTERFACE
    QRCodeGen
    PNG::PNG
)
