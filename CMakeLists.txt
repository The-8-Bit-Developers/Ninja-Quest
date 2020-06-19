# Minimum CMake version
cmake_minimum_required(VERSION 3.1)

# Name project
project(NinjaQuest)

# Add game engine and game
add_subdirectory(8BitEngine)
add_subdirectory(NinjaQuest)