cmake_minimum_required(VERSION 3.0.0)
project(doomy VERSION 0.1.0)

# This project requires C++11
set(CMAKE_CXX_STANDARD 20)

# Automatically find all .cpp files in the src directory
file(GLOB SOURCES "src/*.cpp")

# Add doomy executable
add_executable(doomy ${SOURCES})

# Include our include directory for the shared_graphics and doomy
target_include_directories(doomy PUBLIC include)

# Add and link GLFW
add_subdirectory(lib/GLFW)
target_link_libraries(doomy glfw)

add_subdirectory(lib/Glimpse)
target_link_libraries(doomy Glimpse)

# Set project name and version
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
