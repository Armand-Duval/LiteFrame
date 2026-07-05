cmake_minimum_required(VERSION 3.21)
project(LiteFrameEditor VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(LF_BUILD_QT_APP "Build Qt application" ON)

add_subdirectory(core)

if(LF_BUILD_QT_APP)
    add_subdirectory(app-qt)
endif()
