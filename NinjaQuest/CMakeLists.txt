# Minimum CMake version
cmake_minimum_required(VERSION 3.1)

project(NinjaQuest)
set(subdirectory_path "${CMAKE_CURRENT_LIST_DIR}")

# Debug or release mode
if(NOT CMAKE_BUILD_TYPE MATCHES "Release")
	add_definitions (-DDEBUG)
endif(NOT CMAKE_BUILD_TYPE MATCHES "Release")

# Add files
file (GLOB source_files "${subdirectory_path}/src/*.cpp")
file (GLOB include_files "${subdirectory_path}/include/*.h")
file (GLOB lua_scripts "${subdirectory_path}/../res/*.lua")

# Set flags for GCC
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(STATUS "GCC detected, adding compile flags")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++17 -O2")
endif(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

# Set flags for Visual Studio and put header files in include folder
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	message(STATUS "MSVC detected, adding compile flags")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /W4 /WX /O2")
endif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

# Find Lua
link_directories(${subdirectory_path}/../lua/)

# Find Box2D
link_directories(${subdirectory_path}/../box2d/)

# Add source code to executable and link with engine
include_directories(${PROJECT_NAME} ${subdirectory_path}/include/ ${CMAKE_CURRENT_SOURCE_DIR}/../8BitEngine/include/ ${CMAKE_CURRENT_SOURCE_DIR}/../lua/  ${CMAKE_CURRENT_SOURCE_DIR}/../box2d/)
add_executable(${PROJECT_NAME} ${source_files} ${include_files} ${lua_scripts})
target_link_libraries(${PROJECT_NAME} 8BitEngine)

# Get rid of command prompt window if on Windows and release mode
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	if (debug_or_release_mode MATCHES "Release")
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
	endif (debug_or_release_mode MATCHES "Release")
endif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")