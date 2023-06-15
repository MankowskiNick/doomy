#ifndef GEOM_H
#define GEOM_H

// Maybe add a dist function between 2 vertices


bool Intersect(const Line& l1, const Line& l2);
Vertex FindIntersection(const Line& l1, const Line& l2);

float Dist(const Vertex& v1, const Vertex& v2);
float DistFromOrigin(const Vertex& v);

void Rotate(Vertex& source, float rads);
void Translate(Vertex& source, const Vertex& translation, float scalar);



#endif