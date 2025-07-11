# 
#        d8888 888                .d8888b.                         
#       d88888 888               d88P  Y88b                        
#      d88P888 888               888    888                        
#     d88P 888 88888b.   .d88b.  888        88888b.d88b.  88888b.  
#    d88P  888 888 "88b d8P  Y8b 888        888 "888 "88b 888 "88b 
#   d88P   888 888  888 88888888 888    888 888  888  888 888  888 
#  d8888888888 888 d88P Y8b.     Y88b  d88P 888  888  888 888 d88P 
# d88P     888 88888P"   "Y8888   "Y8888P"  888  888  888 88888P"  
#                                                         888      
# A simple file comparison tool                           888      
#                                                         888              
# Copyright (c) 2025, Abe Mishler
# Licensed under the Universal Permissive License v 1.0
# as shown at https://oss.oracle.com/licenses/upl/.
#

cmake_minimum_required(VERSION 3.10)
message(STATUS "Using CMake version ${CMAKE_VERSION}")

include(FetchContent)

project("AbeCmp" VERSION 0.1.250626)
configure_file(
  "${CMAKE_SOURCE_DIR}/AbeCmpConfig.h.in" 
  "${CMAKE_BINARY_DIR}/AbeCmpConfig.h"
)

# ---- Enable C++20 ----------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# ---- Stick to the standard -------------------------
set(CMAKE_CXX_EXTENSIONS OFF)
# ----------------------------------------------------

# Choose the build type.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type: Release" FORCE)
# set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type: Debug" FORCE)

# Add compile definitions based on the build type.
# This allows us to conditionally compile code based on the build type.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
    add_compile_definitions(DEBUG_BUILD)
else()
    message(STATUS "Building in Release mode")
    add_compile_definitions(RELEASE_BUILD)
endif()

# ---- Source code defined  --------------------------
list(APPEND SRC_CODE
  "src/File.cpp"
  "src/File.h"
  "src/Platform.h"
  "src/Timer.h"
  "src/main.cpp"
)

# Add source code to this project's executable.
add_executable(${CMAKE_PROJECT_NAME} ${SRC_CODE})

# Fetch the AbeArgs library from its Git repository.
# This library is used for command line argument parsing.
FetchContent_Declare(
  AbeArgs
  GIT_REPOSITORY https://github.com/0ABE/abeargs.git
  GIT_TAG        origin/master
  SOURCE_SUBDIR lib
)
FetchContent_MakeAvailable(AbeArgs)
message(STATUS "AbeArgs include dir: " ${ABEARGS_INCLUDE_DIRS})

# Add the current binary directory which contains the generated files.
# Add the include directory for the AbeArgs library.
# This allows the target to find the header files when compiling.
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
                           ${CMAKE_CURRENT_BINARY_DIR}
                           ${ABEARGS_INCLUDE_DIRS})

# Link the AbeArgs library into the target executable.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE AbeArgs)
