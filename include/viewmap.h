#ifndef VIEWMAP_H
#define VIEWMAP_H

#include "common_struct.h"
#include "map.h"
#include "camera.h"
#include "geom.h"


// work out some errors here
class ViewMap {
    public:
        ViewMap(Map& map, Camera& cam) {
            _camera = &cam;
            _map = &map;
        }

        // Perform a translation on the walls to make the illusion that the camera is moving
        void ShiftWalls() {
            for (int i = 0; i < _map->walls.size(); i++) {
                Vertex camera_pos = {
                    .x = _camera->x,
                    .y = _camera->y,
                    .z = _camera->z
                };

                Translate(_map->walls[i].line.v1, camera_pos, -1.0f);
                Translate(_map->walls[i].line.v2, camera_pos, -1.0f);
                
            }
        }

        // Rotate the vertices on the map around the camera to make the illusion the camera is turning
        void RotateWalls() {
            for (int i = 0; i < _map->walls.size(); i++) {
                Rotate(_map->walls[i].line.v1, _camera->angle);
                Rotate(_map->walls[i].line.v2, _camera->angle);
            }
        }

        // Perform translations necessary to create the illusion of looking different directions & moving
        void TranslateMap() {
            ShiftWalls();
            RotateWalls();
        }

    private:
        Camera* _camera;
        Map* _map;

};

#endif