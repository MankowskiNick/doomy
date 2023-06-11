#include "common_struct.h"


// Are the vertices in counter clockwise order?  That is, is the slope of v1->v2 < v1->v3?
bool ccw(Vertex& v1, Vertex& v2, Vertex& v3) {
    return (v3.y - v1.y)*(v2.x - v1.x) > (v2.y - v1.y) * (v3.x - v1.x);
}

// This is a bit of a black box to me, I'd like to explore more of the math behind it.
// Found at https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
// NOTE: This solution does not produce correct results when the two lines are colinear.
// TODO: Find a more general case?
bool Intersect(Line& l1, Line& l2) {
    return ccw(l1.v1, l2.v1, l2.v2) != ccw(l1.v2, l2.v1, l2.v2) 
        && ccw(l1.v1, l1.v2, l2.v1) != ccw(l1.v1, l1.v2, l2.v2);
}