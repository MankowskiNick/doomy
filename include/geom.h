#ifndef GEOM_H
#define GEOM_H

#include "common_struct.h"

bool Intersect(const Line& l1, const Line& l2);

float Dist(const Vertex& v1, const Vertex& v2);
float DistFromOrigin(const Vertex& v);

// Translate source by the scalar * translation
void Translate(Vertex& source, const Vertex& translation, float scalar);

// Rotate a vertex around the origin by rads radians on the xy plane
void Rotate(Vertex& source, float rads);

Vect2<float> VertToVect2(const Vertex& vert); // TODO: Where should these go?
bool VertexEquals(const Vertex& v1, const Vertex& v2);


#endif