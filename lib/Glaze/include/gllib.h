#ifndef gllib_h
#define gllib_h

#include <Glimpse/glimpse.h>
#include <Glimpse/consoleout.h>
#include <Glimpse/status.h>
#include <Glimpse/glerrorcode.h>

#define GLFW_INCLUDE_NONE
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>
#include <shader.h>

#include <utility>
#include <functional>

class GLLib {
    public:
        
        GLLib(
            const char* window_title,
            const int width,
            const int height,
            Glimpse::GlimpseLogger& newLogger
        );

        ~GLLib();

        bool Init(const char* window_name, 
            const int window_width,
            const int window_height,
            void (*error_callback_func)(int, const char*) = NULL, 
            void (*key_callback_func)(GLFWwindow*, int, int, int, int) = NULL,
            void (*mouse_callback_func)(GLFWwindow*, double, double) = NULL
        );

        int GetWidth();
        
        int GetHeight();

        bool BindShader(const std::string& vert_shader_file, const std::string& frag_shader_file);

        void BindKeyCallback(void (*key_callback_func)(GLFWwindow*, int, int, int, int));
        
        void BindMouseCallback(void (*mouse_callback_func)(GLFWwindow*, double, double));

        void BindErrorCallback(void (*error_callback_func)(int, const char*));

        void BufferVerticesWithTextureCoords(float* vertices_head, int size);

        void SwapVAO(int new_index);

        unsigned int GetShaderProgram();

        void DrawTriangles(unsigned int num_vertices);

        // TODO: Depracate this and move to std_graphics for software renderer? seems out of scope for use of this file
        void RenderToScreen();

        GLuint GetVAO();

        GLFWwindow* GetWindow();

        bool WindowShouldClose();

        void Quit();

        Shader* shader;
    private:
        Glimpse::GlimpseLogger* logger;

        GLFWwindow* window;

        int width, height;

        GLuint* AddBuffer(GLuint* buffer, int& size);

        GLuint CurVAO;
        GLuint CurVBO;
        GLuint CurEBO;

        GLuint* VAOs;
        GLuint* VBOs;
        GLuint* EBOs;

        int VAOs_size;
        int VBOs_size;
        int EBOs_size;

};

#endif