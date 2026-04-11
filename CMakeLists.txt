cmake_minimum_required(VERSION 3.15)
project(CyberGuard LANGUAGES CXX)

# Make include directory available
include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)
