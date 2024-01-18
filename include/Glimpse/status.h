#ifndef STATUS_H
#define STATUS_H

#include <string>

enum GLLogStatus : int {
    FATAL,
    WARNING
};


std::string GetGLLogStatusText(const GLLogStatus status);

#endif