# cmake_minimum_required(VERSION 3.0.0)
# project(Glimpse VERSION 0.1.0)

# # This project requires C++20
# set(CMAKE_CXX_STANDARD 20)

# # Automatically find all .cpp files in the src directory
# file(GLOB SOURCES "src/*.cpp")

# # Add library
# add_library(Glimpse ${SOURCES})

# # Include our include directory
# target_include_directories(Glimpse PUBLIC include)

# # Set project name and version
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Glimpse VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Automatically find all .cpp files in the src directory
file(GLOB SOURCES "src/*.cpp")

# Add include directories
include_directories(include)

# Add source files
add_library(Glimpse SHARED ${SOURCES})

# Specify installation rules, if needed
install(TARGETS Glimpse DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

# # Set the location for library installation
# install(TARGETS Glimpse DESTINATION lib)
# install(FILES 
#     include/consoleout.h 
#     include/glerrorcode.h 
#     include/glimpse.h 
#     include/glout.h 
#     include/status.h 
#     DESTINATION include)
