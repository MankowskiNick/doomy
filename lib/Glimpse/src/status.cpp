#include "status.h"

#include <string>

using namespace Glimpse;

std::string Glimpse::GetGLLogStatusText(const GLLogStatus status) {
    switch (status) {
        case FATAL:   return "FATAL";
        case WARNING: return "WARNING";
        default:      return "UNKNOWN";
    }
}