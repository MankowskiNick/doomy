#include "glerrorcode.h"

#include <string>

std::string GetGLErrorString(GLErrorCode errorCode) {
    switch (errorCode) {
        case GLErrorCode::GL_NO_ERROR:
            return "GL_NO_ERROR";
        case GLErrorCode::GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GLErrorCode::GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GLErrorCode::GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GLErrorCode::GL_STACK_OVERFLOW:
            return "GL_STACK_OVERFLOW";
        case GLErrorCode::GL_STACK_UNDERFLOW:
            return "GL_STACK_UNDERFLOW";
        case GLErrorCode::GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        case GLErrorCode::GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GLErrorCode::GL_CONTEXT_LOST:
            return "GL_CONTEXT_LOST";
        default:
            return "Unknown Error";
    }
}