cmake_minimum_required(VERSION 3.0.0)
project(doomy VERSION 0.1.0)

# This project required C++11
set(CMAKE_CXX_STANDARD 11)

include(CTest)
enable_testing()

#Add shared_graphics library
add_library(shared_graphics SHARED include/shared_graphics.h src/shared_graphics.cpp)

# Add doomy executable
add_executable(doomy src/main.cpp src/glad.cpp src/draw.cpp src/std_graphics.cpp src/geom.cpp src/input.cpp)

# Include our include directory for the shared_graphics and doomy
target_include_directories(doomy PUBLIC include)
target_include_directories(shared_graphics PUBLIC include)

# Link shared_graphics to doomy
target_link_libraries(doomy shared_graphics)

# Add and link GLFW
add_subdirectory(lib/GLFW)
target_link_libraries(doomy glfw)

# Set the output directory for shared_graphics.so
set_target_properties(shared_graphics PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Make sure editor/bin exists and copy shared_graphics to the level editor bin
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/editor/bin)
add_custom_command(TARGET shared_graphics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:shared_graphics> ${CMAKE_CURRENT_SOURCE_DIR}/editor/bin)

# Set project name and version
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
