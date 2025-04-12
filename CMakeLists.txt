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

cmake_minimum_required(VERSION 3.8)
message(STATUS "Using CMake version ${CMAKE_VERSION}")

project("AbeCmp" VERSION 0.1.250308)

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

# ---- Choose a build type ---------------------------
#
# NOTE: To set a build type using CMake Tools (VSCode
# extension), comment these lines. If not using CMake
# Tools, uncomment the following to set a build type.
# 
# * * Don't forget the #define for Debug builds. * * *
#
# ---- Debug (with a #define DEBUG_BUILD) ------------
# set(CMAKE_BUILD_TYPE Debug)
# add_compile_definitions(DEBUG_BUILD)
# ---- Release ---------------------------------------
# set(CMAKE_BUILD_TYPE Release)
# ----------------------------------------------------

# ---- Source code defined  --------------------------
list(APPEND SRC_CODE
  "abecmp.cpp"
  "File.cpp"
  "File.h"
  "Platform.h"
)

# Path to the AbeArgs library
link_directories(/usr/lib/abeargs/)

# Add source code to this project's executable.
add_executable(${CMAKE_PROJECT_NAME} ${SRC_CODE})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

# Link the following libraries into the executable.
target_link_libraries(${CMAKE_PROJECT_NAME} libAbeArgs.a)
