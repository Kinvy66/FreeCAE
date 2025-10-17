cmake_minimum_required(VERSION 3.12)

project(FreeCAE LANGUAGES CXX C)

set(FREECAE_LIB_DIR ${CMAKE_SOURCE_DIR}/lib CACHE PATH "Path to prebuilt libraries")
set(FREECAE_3RDPARTY_DIR ${CMAKE_SOURCE_DIR}/3rdparty CACHE PATH "Path to 3rdparty sources")


add_subdirectory(src)

