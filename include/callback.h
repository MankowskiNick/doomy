#ifndef CALLBACK_H
#define CALLBACK_H

#include "constants.h"
#include "camera.h"

class GLLib; // TODO: Sort this out? Maybe?

class CallbackHandler {

    public:
        CallbackHandler(Camera& newCamera, GLLib& gl);
        ~CallbackHandler();

        // Callback method -- these probably have to exist outside of the class definition
        void KeyBind(GLFWwindow* window, int key, int scancode, int action, int mods);
        void MouseBind(GLFWwindow* window, double xpos, double ypos);
        void ErrorBind(int error, const char* description);

        // Getter to grab the current key, used by InputHandler
        int* GetCurKey() const;

    private:

         // Current key being pressed, if NULL, no key is pressed
        int* cur_key;

        // Pointer to the camera object coming from main
        Camera* camera_ptr;

        // Current mouse position
        double cur_mousex, cur_mousey;
};

void ConfigureCallbacks(CallbackHandler* newCallbackHandler, GLLib& gl); // This is kinda janky

#endif