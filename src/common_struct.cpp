#include "common_struct.h"
#include "vectmath.h"

Vect2<float> VertToVect2(const Vertex& vert) {
    return Vect2<float> {
        .a = vert.x,
        .b = vert.y
    };
}
