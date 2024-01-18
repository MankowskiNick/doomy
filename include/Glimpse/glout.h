#ifndef GLOUT_H
#define GLOUT_H

#include <string>
#include "status.h"

class GLOut {
public:
    virtual ~GLOut() = 0;

    virtual void Output(const std::string& message, const GLLogStatus status) = 0;
};

#endif