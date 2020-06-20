# Minimum CMake version
cmake_minimum_required(VERSION 3.1)

project(8BitEngine)
set(subdirectory_path "${CMAKE_CURRENT_LIST_DIR}")

# Debug or release mode
if(NOT CMAKE_BUILD_TYPE MATCHES "Release")
	add_definitions (-DDEBUG)
endif(NOT CMAKE_BUILD_TYPE MATCHES "Release")

# Add files
file (GLOB source_files "${subdirectory_path}/src/*.cpp")
file (GLOB include_files "${subdirectory_path}/include/*.h")

# Set flags for GCC
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(STATUS "GCC detected, adding compile flags")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++17 -O2")
endif(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

# On MacOS we need to tell CMake where the dylibs are
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(DYLD_LIBRARY_PATH, "${subdirectory_path}/../lua/:${subdirectory_path}/../SFML/:${subdirectory_path}/../box2d/")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Set flags for Visual Studio and put header files in include folder
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	message(STATUS "MSVC detected, adding compile flags")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /W4 /WX /O2")
endif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

# Set SFML to link statically if not on Mac OS
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(SFML_STATIC_LIBRARIES TRUE)
	set(CMAKE_FIND_FRAMEWORK LAST)
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Find SFML - main is required unless on MacOS or Linux
set(SFML_DIR "${subdirectory_path}/../SFML/lib/cmake/SFML")
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	find_package(SFML 2.5 COMPONENTS graphics audio main REQUIRED)
endif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
if (UNIX AND NOT APPLE)
	include_directories(${PROJECT_NAME} ${subdirectory_path}/../SFML/)
	link_directories(${subdirectory_path}/../SFML/lib/)
endif()

# Find Lua
link_directories(${subdirectory_path}/../lua/)

# Find Box2D
link_directories(${subdirectory_path}/../box2d/)

# Add source code to executable and link with SFML and Lua
include_directories(${PROJECT_NAME} ${subdirectory_path}/include/ ${subdirectory_path}/../lua/  ${subdirectory_path}/../box2d/)
#add_executable(${PROJECT_NAME} ${source_files} ${include_files} ${lua_source_files} ${lua_scripts})
add_library(${PROJECT_NAME} ${source_files} ${include_files} ${lua_source_files})

if (NOT UNIX)
	target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio sfml-main lua53)
endif (NOT UNIX)

if (UNIX)
	target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-audio sfml-system lua53)
endif (UNIX)

# Add box-2d
if(NOT CMAKE_BUILD_TYPE MATCHES "Release" AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	target_link_libraries(${PROJECT_NAME} box2d-d)
else(NOT CMAKE_BUILD_TYPE MATCHES "Release" AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	target_link_libraries(${PROJECT_NAME} box2d)
endif(NOT CMAKE_BUILD_TYPE MATCHES "Release" AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Get rid of command prompt window if on Windows and release mode
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	if (debug_or_release_mode MATCHES "Release")
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
	endif (debug_or_release_mode MATCHES "Release")
endif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")