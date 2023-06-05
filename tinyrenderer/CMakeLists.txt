cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(Tinyrender)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# 设置静态库文件目录
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 动态库文件目录
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 运行文件目录
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# add_library(tga STATIC tgaimage.cpp tgaimage.h)
# add_library(model STATIC model.cpp model.h geometry.h geometry.cpp)
# add_executable(render main.cpp)
# target_link_libraries(render tga model)


set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB SOURCES
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.cpp"
)
add_executable(render ${SOURCES})
target_include_directories(render PRIVATE "${SRC_DIR}")