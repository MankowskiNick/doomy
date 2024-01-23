#ifndef CONSOLEOUT_H
#define CONSOLEOUT_H

#include "glout.h"

#include <string>

class ConsoleOut : public Glimpse::GLOut {
public:
    ConsoleOut();
    ~ConsoleOut() override;

    void Output(const std::string& message, const Glimpse::GLLogStatus status)  override;
};

#endif