cmake_minimum_required(VERSION 3.25)

project(mongoose)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
    mongoose/mongoose.c
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
)