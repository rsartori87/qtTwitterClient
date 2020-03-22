cmake_minimum_required(VERSION 3.5)

project(twitterLib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Network NetworkAuth REQUIRED)

add_library(twitterLib SHARED
  twitterLib_global.h
  twitter.cpp
  twitter.h
)

target_include_directories(twitterLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(twitterLib PRIVATE Qt5::Core Qt5::Network Qt5::NetworkAuth)

target_compile_definitions(twitterLib PRIVATE TWITTERLIB_LIBRARY)
