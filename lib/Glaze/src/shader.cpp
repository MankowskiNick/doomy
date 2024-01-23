
#include "shader.h"

Shader::Shader(Glimpse::GlimpseLogger& newLogger) {
    vShader = 0;
    fShader = 0;
    logger = &newLogger;
}

Shader::Shader(
    const std::string& vert_shader_file, 
    const std::string& frag_shader_file, 
    Glimpse::GlimpseLogger& newLogger
) {
    logger = &newLogger;
    CompileShader(vert_shader_file, frag_shader_file);
}

Shader::~Shader() {
    if (vShader != 0)
        glDeleteShader(vShader);
    if (fShader != 0)
        glDeleteShader(fShader);
}

unsigned int Shader::GetProgramID() { return id; }

void Shader::Use() { glUseProgram(id); }

void Shader::SetBool(const char* name, bool value) const { glUniform1i(glGetUniformLocation(id, name), (int)value); }        

void Shader::SetInt(const char* name, int value) const { glUniform1i(glGetUniformLocation(id, name), value); }

void Shader::SetFloat(const char* name, float value) const { glUniform1f(glGetUniformLocation(id, name), value); }

void Shader::CompileShader(const std::string& vert_shader_file, const std::string& frag_shader_file) {
    std::string load_error_msg = "";

    std::string vert_shader_source = "";
    LoadSource(GL_VERTEX_SHADER, vert_shader_source, vert_shader_file, load_error_msg);
    std::string frag_shader_source = "";
    LoadSource(GL_FRAGMENT_SHADER, frag_shader_source, frag_shader_file, load_error_msg);

    if (load_error_msg != "") {
        logger->Log("ERROR: Failed to load one or more source files.  " + load_error_msg, Glimpse::FATAL);
        exit(1);
    }

    id = CreateShader(vert_shader_source, frag_shader_source);
}
        
void Shader::LoadSource(const unsigned int type, std::string& source, const std::string& file_name, std::string &error_msg) {
    std::ifstream fin;
    if (type == GL_VERTEX_SHADER) {
        fin.open(file_name.data(), std::ios::in);
        if (!fin.is_open()) {
            error_msg += "Error: Vertex shader source file doesn't exist.  (" + file_name + ") \n";
            fin.close();
            return;
        }
    }
    else if (type == GL_FRAGMENT_SHADER) {
        fin.open(file_name.data(), std::ios::in);
        if (!fin) {
            error_msg += "Error: Fragment shader source file doesn't exist.  (" + file_name + ") \n";
            fin.close();
            return;
        }
    }
    while (!fin.eof()) {
        std::string tmp;
        getline(fin, tmp);
        source += tmp + "\n";
    }
    fin.close();
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    vShader = Compile(GL_VERTEX_SHADER, vertexShader);
    fShader = Compile(GL_FRAGMENT_SHADER, fragmentShader);

    // Link shaders into one program
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);

    glLinkProgram(program);

    glValidateProgram(program);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return program;
}

unsigned int Shader::Compile(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = &source[0];
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    // Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        glDeleteShader(id);

        logger->Log("Error compiling shaders.  " + std::string(message), Glimpse::FATAL);

        return 0;
    }

    return id;
}