#include "glimpse.h"

using namespace Glimpse;

GlimpseLogger::GlimpseLogger() {
    GlimpseLogger(NULL);
}

GlimpseLogger::GlimpseLogger(GLOut* newOutput) {
    this->AddLogging(newOutput);
}

GlimpseLogger::~GlimpseLogger() {
    for (int i = 0; i < gLogOutputs.size(); i++)
        delete gLogOutputs[i];
}

void GlimpseLogger::AddLogging(GLOut* newOutput) {
    if (newOutput != NULL)
        gLogOutputs.push_back(newOutput);
}

void GlimpseLogger::Log(const std::string& message, GLLogStatus status) {
    for (int i = 0; i < gLogOutputs.size(); i++)
        gLogOutputs[i]->Output(message, status);
    if (status == FATAL)
        exit(status);
}

void GlimpseLogger::LogGL(int code) {
    std::string errorMessage = "OpenGL Error - " + GetGLErrorString(code);
    this->Log(errorMessage, FATAL);
}