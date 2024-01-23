
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Glaze VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Automatically find all .cpp files in the src directory
file(GLOB SOURCES "src/*.cpp")

# Add include directories
include_directories(include)

# Add source files
add_library(Glaze SHARED ${SOURCES})

add_subdirectory(lib/GLFW)
target_link_libraries(Glaze glfw)

target_link_libraries(Glaze Glimpse)

# Specify installation rules, if needed
install(TARGETS Glaze DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
