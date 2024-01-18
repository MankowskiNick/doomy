#include "status.h"

#include <string>

std::string GetGLLogStatusText(const GLLogStatus status) {
    switch (status) {
        case FATAL:   return "FATAL";
        case WARNING: return "WARNING";
        default:      return "UNKNOWN";
    }
}