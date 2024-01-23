
#include <gllib.h>
#define GLFW_INCLUDE_NONE
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>

#include <shader.h>

#include <shared_graphics.h>

GLLib::GLLib(
    const char* window_title,
    const int width,
    const int height,
    Glimpse::GlimpseLogger& newLogger
) { 
    VAOs = new GLuint;
    VBOs = new GLuint;
    EBOs = new GLuint;
            
    VAOs_size = 0;
    VBOs_size = 0;
    EBOs_size = 0;

    logger = &newLogger;

    Init(window_title, width, height);
}

GLLib::~GLLib() { }

bool GLLib::Init(const char* window_name, 
    const int window_width,
    const int window_height,
    void (*error_callback_func)(int, const char*), 
    void (*key_callback_func)(GLFWwindow*, int, int, int, int),
    void (*mouse_callback_func)(GLFWwindow*, double, double)
) {
    try {
        // Init GLFW
        if (!glfwInit()) 
            return false;
            
        // Window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef __APPLE__ 
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        // Create window
        width = window_width;
        height = window_height;
        window = glfwCreateWindow(width, height, window_name, NULL, NULL);

        // Window creation failure
        if (!window) {
            logger->Log("Failed to create window.", Glimpse::FATAL);
            glfwTerminate();
            return false;
        }


        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

        BindErrorCallback(error_callback_func);
        BindKeyCallback(key_callback_func);
        BindMouseCallback(mouse_callback_func);

        // Set OpenGL context
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        // Initialize GLAD
        gladLoadGL();    

        // Glad init failure
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            logger->Log("Failed to initialize OpenGL context.", Glimpse::FATAL);
            return false;
        }

        glEnable(GL_DEPTH_TEST);

        return true;
    }
    catch(...) {
        logger->Log("Failed to initialize OpenGL context.", Glimpse::FATAL);
        return false;
    }
}
    
int GLLib::GetWidth() { return width; }
        
int GLLib::GetHeight() { return height; }

bool GLLib::BindShader(const std::string& vert_shader_file, const std::string& frag_shader_file) {
    try {
        // Compile vertex shader
        shader = new Shader(vert_shader_file, frag_shader_file, *logger);
        shader->Use();
        return true; 
    }
    catch(...) {
        return false;
    }
}
        
void GLLib::BindKeyCallback(void (*key_callback_func)(GLFWwindow*, int, int, int, int)) {
    if (key_callback_func != NULL)
        glfwSetKeyCallback(window, key_callback_func);
}
        
void GLLib::BindMouseCallback(void (*mouse_callback_func)(GLFWwindow*, double, double)) {
    if (mouse_callback_func != NULL)
        glfwSetCursorPosCallback(window, mouse_callback_func);
}

void GLLib::BindErrorCallback(void (*error_callback_func)(int, const char*)) {
    if (error_callback_func != NULL) 
        glfwSetErrorCallback(error_callback_func);
}

void GLLib::BufferVerticesWithTextureCoords(float* vertices_head, int size) {
    GLuint* new_vao_ptr = AddBuffer(VAOs, VAOs_size);
    glGenVertexArrays(1, new_vao_ptr);
    glBindVertexArray(*new_vao_ptr);

    GLuint* new_vbo_ptr = AddBuffer(VBOs, VBOs_size);
    glGenBuffers(1, new_vbo_ptr);

    glBindBuffer(GL_ARRAY_BUFFER, *new_vbo_ptr); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices_head, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void GLLib::SwapVAO(int new_index) {
    GLuint new_vao = *(VAOs + new_index * sizeof(int));
    CurVAO = new_vao;
    glBindVertexArray(new_vao);
}

unsigned int GLLib::GetShaderProgram() {
    return shader->GetProgramID();
}

void GLLib::DrawTriangles(unsigned int num_vertices) {
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}

void GLLib::RenderToScreen() {
    SwapVAO(0);
    DrawTriangles(6);
}

GLuint GLLib::GetVAO() {
    return CurVAO;
}

GLFWwindow* GLLib::GetWindow() {
    return window;
}

bool GLLib::WindowShouldClose() {
    return glfwWindowShouldClose(GetWindow());
}

GLuint* GLLib::AddBuffer(GLuint* buffer, int& size) {

    GLuint* end_ptr = (buffer + (size * sizeof(GLuint)));
    *end_ptr = GLuint();
    size++;

    return end_ptr;
}

void GLLib::Quit() {
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}