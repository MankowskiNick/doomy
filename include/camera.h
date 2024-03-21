#ifndef CAMERA_H
#define CAMERA_H
#include <math.h>



class Camera {
    public:

        Camera() {
            Camera(0.0f, 1.0f, 0.0f, 0.0f);
        }

        Camera(float new_x, float new_y, float new_z, float new_angle) {
            x = new_x;
            y = new_y;
            z = new_z;
            angle = new_angle;
            //fov = new_fov;
            SPEED = 0.01f;
        }

        ~Camera() {}

        void TurnLeft() {
            angle -= M_PI / 60;
        }

        void TurnRight() {
            angle += M_PI / 60;
        }

        void MoveForward() {
            x += SPEED * cos(angle + (M_PI / 2.0f));
            y += SPEED * sin(angle + (M_PI / 2.0f));
        }

        void MoveBackward() {
            x -= SPEED * cos(angle + (M_PI / 2.0f));
            y -= SPEED * sin(angle + (M_PI / 2.0f));
        }

        void MoveLeft() {
            x -= SPEED * sin(angle + (M_PI / 2.0f));
            y += SPEED * cos(angle + (M_PI / 2.0f));
        }

        void MoveRight() {
            x += SPEED * sin(angle + (M_PI / 2.0f));
            y -= SPEED * cos(angle + (M_PI / 2.0f));
        }

        void Reset() {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
            angle = 0.0f;
        }

        float x, y, z;
        float angle;
        //float fov;
        float SPEED;
};

#endif