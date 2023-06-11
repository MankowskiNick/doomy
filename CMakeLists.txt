cmake_minimum_required(VERSION 3.0.0)
project(doomy VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(doomy src/main.cpp src/glad.cpp src/draw.cpp src/std_graphics.cpp src/geom.cpp)

include_directories(include)

add_subdirectory(lib/GLFW)
target_link_libraries(doomy glfw)
#find_package(doomy REQUIRED)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
