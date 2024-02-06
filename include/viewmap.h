#ifndef VIEWMAP_H
#define VIEWMAP_H

#include "common_struct.h"
#include "map.h"
#include "camera.h"
#include "geom.h"
#include "vectmath.h"


// TODO: work out some errors here(maybe)
class ViewMap {
    public:
        ViewMap() {}

        ViewMap(Camera& cam) {
            _camera = &cam;
        }

        // Perform a translation on the walls to make the illusion that the camera is moving
        void ShiftWalls() {
            for (int i = 0; i < _map->walls.size(); i++) {
                Vertex camera_pos = {
                    .x = _camera->x,
                    .y = _camera->y
                };

                Translate(_map->walls[i].line.v1, camera_pos, -1.0f);
                Translate(_map->walls[i].line.v2, camera_pos, -1.0f);
                
                _map->walls[i].max_height -= _camera->z;
                _map->walls[i].min_height -= _camera->z;
                _map->walls[i].floor_height -= _camera->z;
                _map->walls[i].ceiling_height -= _camera->z;
            }
        }

        // Rotate the vertices on the map around the camera to make the illusion the camera is turning
        void RotateWalls() {
            for (int i = 0; i < _map->walls.size(); i++) {
                Rotate(_map->walls[i].line.v1, _camera->angle);
                Rotate(_map->walls[i].line.v2, _camera->angle);
            }
        }

        void LoadMap(Map& map) {
            _map = &map;
        }

        // Perform translations necessary to create the illusion of looking different directions & moving
        void TranslateMap() {
            ShiftWalls();
            RotateWalls();
        }

        Map* GetMap() {
            return _map;
        }

    private:
        Camera* _camera;
        Map* _map;

};

#endif