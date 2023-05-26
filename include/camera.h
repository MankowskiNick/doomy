#ifndef CAMERA_H
#define CAMERA_H
#include <math.h>



class Camera {
    public:

        Camera() {
            Camera(0.0f, 0.0f, 0.0f, 0.0f, M_PI / 3);
        }

        Camera(float new_x, float new_y, float new_z, float new_angle, float new_fov) {
            x = new_x;
            y = new_y;
            z = new_z;
            angle = new_angle;
            fov = new_fov;
            SPEED = 0.04f;
        }

        ~Camera() {}

        void MoveForward() {
            x += SPEED * cos(angle);
            y += SPEED * sin(angle);
        }

        void MoveBackward() {
            x -= SPEED * cos(angle);
            y -= SPEED * sin(angle);
        }

        void MoveLeft() {
            x -= SPEED * sin(angle);
            y += SPEED * cos(angle);
        }

        void MoveRight() {
            x += SPEED * sin(angle);
            y -= SPEED * cos(angle);
        }

        float x, y, z;
        float angle;
        float fov;
        float SPEED;
};

#endif