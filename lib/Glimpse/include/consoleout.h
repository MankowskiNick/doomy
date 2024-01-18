#ifndef CONSOLEOUT_H
#define CONSOLEOUT_H

#include "glout.h"

#include <string>

class ConsoleOut : public GLOut {
public:
    ConsoleOut();
    ~ConsoleOut() override;

    void Output(const std::string& message, const GLLogStatus status)  override;
};

#endif