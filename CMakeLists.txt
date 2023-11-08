cmake_minimum_required(VERSION 3.0.0)
project(doomy VERSION 0.1.0)

# This project requires C++11
set(CMAKE_CXX_STANDARD 11)

include(CTest)
enable_testing()

# Add doomy executable
add_executable(doomy src/main.cpp src/input.cpp src/callback.cpp)

# Include our include directory for the shared_graphics and doomy
target_include_directories(doomy PUBLIC include)

# Add and link Graphics
add_subdirectory(lib/graphics)
target_link_libraries(doomy graphics)

# Set project name and version
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
