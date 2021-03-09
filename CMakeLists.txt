cmake_minimum_required(VERSION 3.12)
project(AlphabetConverter)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

include_directories( "${PROJECT_SOURCE_DIR}/Src"
                     "${PROJECT_SOURCE_DIR}/Util"
                     )

add_subdirectory(Src)
add_subdirectory(Util)