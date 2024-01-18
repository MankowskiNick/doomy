#ifndef GLERRORCODE_H
#define GLERRORCODE_H

#include <string>
#include <glad/glad.h>

namespace Glimpse {
    std::string GetGLErrorString(int errorCode);
}
#endif
