#ifndef SHADER_H
#define SHADER_H

#include <Glimpse/glimpse.h>
#include <Glimpse/consoleout.h>
#include <Glimpse/status.h>
#include <Glimpse/glerrorcode.h>

#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>

class Shader {
    public:
        Shader(Glimpse::GlimpseLogger& newLogger);

        Shader(
            const std::string& vert_shader_file, 
            const std::string& frag_shader_file, 
            Glimpse::GlimpseLogger& newLogger
        );

        ~Shader();

        unsigned int GetProgramID();

        void Use();

        void SetBool(const char* name, bool value) const;   
        void SetInt(const char* name, int value) const;
        void SetFloat(const char* name, float value) const;

        void CompileShader(const std::string& vert_shader_file, const std::string& frag_shader_file);
        
    private:
        Glimpse::GlimpseLogger* logger;

        unsigned int id, vShader, fShader;
        
        void LoadSource(const unsigned int type, std::string& source, const std::string& file_name, std::string &error_msg);

        unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

        unsigned int Compile(unsigned int type, const std::string& source);
};

#endif