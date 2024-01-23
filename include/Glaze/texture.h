#ifndef TEXTURE_H
#define TEXTURE_H

#include "gllib.h"

class Texture {
    public:

        Texture();

        Texture(
            unsigned char* data, 
            int new_width, 
            int new_height, 
            GLLib* gl_lib, 
            int new_id, 
            unsigned int gl_texture, 
            const char* uniform
        );

        ~Texture();

        // Don't allow use of gl_texture = GL_TEXTURE0
        void LoadData(
            unsigned char* new_data, 
            int new_width, 
            int new_height, 
            GLLib* gl_lib, 
            int new_id, 
            unsigned int gl_texture, 
            const char* uniform
        );

        void Activate(GLLib* gl_lib, const char* uniform);

        void UpdateTextureData(unsigned char* new_data);

        unsigned int GetTextureID();

    private:
        int id;
        unsigned int texture;
        int width, height;
        unsigned char* data;

};

#endif