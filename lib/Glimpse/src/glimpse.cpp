#include "glimpse.h"

Glimpse::Glimpse() {
    Glimpse(NULL);
}

Glimpse::Glimpse(GLOut* newOutput) {
    this->AddLogging(newOutput);
}

Glimpse::~Glimpse() {
    for (int i = 0; i < gLogOutputs.size(); i++)
        delete gLogOutputs[i];
}

void Glimpse::AddLogging(GLOut* newOutput) {
    if (newOutput != NULL)
        gLogOutputs.push_back(newOutput);
}

void Glimpse::Log(const std::string& message, GLLogStatus status) {
    for (int i = 0; i < gLogOutputs.size(); i++)
        gLogOutputs[i]->Output(message, status);
    if (status == FATAL)
        exit(status);
}

void Glimpse::LogGL(GLErrorCode code) {
    std::string errorMessage = "OpenGL Error - " + GetGLErrorString(code);
    this->Log(errorMessage, FATAL);
}