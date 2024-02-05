cmake_minimum_required(VERSION 3.22) # The minimum CMake version used.
project(Unicode) # Name of the current project.

add_executable(Unicode) # Define a name for executable.

set(target Unicode) # Create a name for target.

#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")

# Add the files to add to executable.
target_sources(${target}
        PRIVATE
        main.cpp # The main file of the project.

        ${CMAKE_SOURCE_DIR}/include/unicode.h

        ${CMAKE_SOURCE_DIR}/src/unicode.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include) # Instruct the compiler where to look for the given header files within this project.
include_directories(${CMAKE_SOURCE_DIR}/src) # Instruct the compiler where to look for the given src files within this project.