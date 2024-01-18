#ifndef GLIMPSE_H
#define GLIMPSE_H

#include "glout.h"
#include "status.h"
#include "glerrorcode.h"

#include <string>
#include <vector>

/// Once Glimpse instance per OpenGL application
/// Log standard errors as well as OpenGL errors
/// Define custom outputs for logging(I am thinking text files/console output?)
/// Logging hierarchy
///     -Fatal: Log and crash program, spit out stack trace
///     -Noncritical: Just log, sort of like a warning(optional spit out stack trace?)

class Glimpse {
public:
    Glimpse();
    Glimpse(GLOut* newOutput);
    ~Glimpse();

    void AddLogging(GLOut* newOutput);

    void Log(const std::string& message, GLLogStatus status);
    void LogGL(const GLErrorCode code);

private:
    std::vector<GLOut*> gLogOutputs;
};

#endif