cmake_minimum_required(VERSION 3.15)
project(IpaLocalInstallServer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置静态库文件目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/lib)

# 动态库文件目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/lib)

# 可执行文件目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/bin)

# 设置 debug/release 库文件后缀
set(CMAKE_DEBUG_POSTFIX "_debug")    
set(CMAKE_RELEASE_POSTFIX "_release") 

add_subdirectory(source/third-party/mongoose)
add_subdirectory(source/third-party/QrCodeGen)
add_subdirectory(source/IpaInstallation)

add_executable(IpaLocalInstallServer source/main.cpp)

target_link_libraries(${PROJECT_NAME} 
    mongoose 
    QRCodeGen
    IpaInstallation
)
