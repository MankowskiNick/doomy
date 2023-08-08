add_library(graphics SHARED
    draw.h
    gllib.h
    minimap.h
    Shader.h
    shared_graphics.h
    std_graphics.h
    src/bsp_tree.cpp
    src/draw.cpp
    src/geom.cpp
    src/glad.cpp
    src/minimap.cpp
    src/std_graphics.cpp
    src/wall_node.cpp
    )
    
# Link GLFW to the Graphics library
add_subdirectory(lib/GLFW)  # Assuming GLFW is in root/lib/GLFW
target_link_libraries(graphics glfw)

target_include_directories(graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/include
)
