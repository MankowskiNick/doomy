#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>
#include <functional>

#include "camera.h"
#include "constants.h"
#include "callback.h"

class InputHandler {
    public:
        InputHandler(Camera& newCamera ,CallbackHandler& newCallbackHandler);

        ~InputHandler();

        void PerformKeyAction();

    private:
        // Configure Input
        void ConfigureInput(Camera& ref_camera);

        // Key-Action Map
        std::unordered_map<int, std::function<void()> > action_map;

        // Pointer to the camera object coming from main
        Camera* camera_ptr;

        CallbackHandler* callbackHandler;
};

#endif