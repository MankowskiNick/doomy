#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "gllib.h"

class Texture {
    public:

        Texture() {}

        Texture(unsigned char* data, int new_width, int new_height, GLLib* gl_lib, int new_id, unsigned int gl_texture, const char* uniform) {
            LoadData(data, new_width, new_height, gl_lib, new_id, gl_texture, uniform);
        }

        ~Texture() {}

        // Don't allow use of gl_texture = GL_TEXTURE0
        void LoadData(unsigned char* new_data, int new_width, int new_height, GLLib* gl_lib, int new_id, unsigned int gl_texture, const char* uniform) {

            id = new_id;
            width = new_width;
            height = new_height;
            data = new_data;

            glGenTextures(1, &texture);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  
            glGenerateMipmap(GL_TEXTURE_2D);

            glActiveTexture(gl_texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            Activate(gl_lib, uniform);
        }

        void Activate(GLLib* gl_lib, const char* uniform) {    
            gl_lib->shader.SetInt(uniform, id);
        }

        void UpdateTextureData(unsigned char* new_data) {
            data = new_data;
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        unsigned int GetTextureID() {
            return texture;
        }


    private:
        int id;
        unsigned int texture;
        int width, height;
        unsigned char* data;

};

#endif