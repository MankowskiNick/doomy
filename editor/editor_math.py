from map import *

ERROR_MARGIN = 1e-6

def extend_line(line, scalar=100):
    # Calculate the direction vector between the two vertices
    direction_vector = Vertex(line.v2.id,
                              line.v2.x - line.v1.x,
                              line.v2.y - line.v1.y,
                              line.v2.z - line.v1.z)

    # Calculate the extension for each vertex
    ext_x = direction_vector.x * scalar
    ext_y = direction_vector.y * scalar
    ext_z = direction_vector.z * scalar

    # Create the new extended vertices
    ext_v1 = Vertex(line.v1.id, line.v1.x - ext_x, line.v1.y - ext_y, line.v1.z - ext_z)
    ext_v2 = Vertex(line.v2.id, line.v2.x + ext_x, line.v2.y + ext_y, line.v2.z + ext_z)

    # Create and return the extended line
    extended_line = Line(ext_v1, ext_v2)
    return extended_line

def get_midpoint(wall):
    return Vertex(-1, (wall.line.v1.x + wall.line.v2.x) / 2, 
                    (wall.line.v1.y + wall.line.v2.y) / 2, 
                    (wall.line.v1.z + wall.line.v2.z) / 2)

def vertex_equals(v1, v2):
    if (v1 is None or v2 is None):
        return False
    return abs(v1.x - v2.x) < ERROR_MARGIN and abs(v1.y - v2.y) < ERROR_MARGIN and abs(v1.z - v2.z) < ERROR_MARGIN

def is_front(vertex, wall):
    line_vect = Vect3(wall.line.v2.x - wall.line.v1.x, wall.line.v2.y - wall.line.v1.y, wall.line.v2.z - wall.line.v1.z)
    v1_to_vert_vect = Vect3(vertex.x - wall.line.v1.x, vertex.y - wall.line.v1.y, vertex.z - wall.line.v1.z)

    cross_product = CrossProduct(line_vect, v1_to_vert_vect)

    return cross_product.c > 0

def is_front_walls(wall1, wall2):
    midpoint = get_midpoint(wall1)
    return is_front(midpoint, wall2)

# Are the vertices in counter clockwise order?  That is, is the slope of v1->v2 < v1->v3?
def ccw(v1, v2, v3):
    return (v3.y - v1.y)*(v2.x - v1.x) > (v2.y - v1.y) * (v3.x - v1.x);

# This is a bit of a black box to me, I'd like to explore more of the math behind it.
# Found at https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
# NOTE: This solution does not produce correct results when the two lines are colinear.
# TODO: Find a more general case?
def intersect(l1, l2):
    if abs(l1.v1.z - l2.v1.z) < ERROR_MARGIN:
        return ccw(l1.v1, l2.v1, l2.v2) != ccw(l1.v2, l2.v1, l2.v2) and ccw(l1.v1, l1.v2, l2.v1) != ccw(l1.v1, l1.v2, l2.v2);
    return False

# Return the intersection of 2 lines
def find_intersection(l1, l2):
    if not intersect(l1, l2):
        return None

    AB = Vect2(l1.v1.x - l1.v2.x, l1.v1.y - l1.v2.y)
    CD = Vect2(l2.v1.x - l2.v2.x, l2.v1.y - l2.v2.y)

    A = Vect2(l1.v1.x, l1.v1.y)
    C = Vect2(l2.v1.x, l2.v1.y)

    CminA = Add(C, Scale(A, -1.0))
    AB_CD = Matrix2x2(AB.a, CD.a, AB.b, CD.b)

    t = Mult(Inverse(AB_CD), CminA)

    result = Add(A, Scale(AB, t.a))

    intersection = Vertex(-1, result.a, result.b, l1.v1.z)
    return intersection

def Midpoint(wall):
    return Vertex(
        id=0,
        x=(wall.line.v1.x + wall.line.v2.x) / 2.0,
        y=(wall.line.v1.y + wall.line.v2.y) / 2.0
    )


class Matrix2x2:
    def __init__(self, a, b, c, d):
        self.a = a
        self.b = b
        self.c = c
        self.d = d

class Matrix3x3:
    def __init__(self, a, b, c, d, e, f, g, h, i):
        self.a = a
        self.b = b
        self.c = c
        self.d = d
        self.e = e
        self.f = f
        self.g = g
        self.h = h
        self.i = i

class Vect2:
    def __init__(self, a, b):
        self.a = a
        self.b = b

class Vect3:
    def __init__(self, a, b, c):
        self.a = a
        self.b = b
        self.c = c

def Det(mat):
    return (mat.a * mat.d) - (mat.c * mat.b)

def Det3x3(mat):
    A = Matrix2x2(mat.e, mat.f, mat.h, mat.i)
    B = Matrix2x2(mat.d, mat.f, mat.g, mat.i)
    C = Matrix2x2(mat.d, mat.e, mat.g, mat.h)
    return (mat.a * Det(A)) - (mat.b * Det(B)) + (mat.c * Det(C))

def Inverse(mat):
    inverse_det = 1 / Det(mat)

    return Matrix2x2(
        inverse_det * mat.d, -1 * inverse_det * mat.b,
        -1 * inverse_det * mat.c, inverse_det * mat.a
    )

def Scale(vect, scalar):
    return Vect2(scalar * vect.a, scalar * vect.b)

def Mult(mat, vect):
    return Vect2(
        mat.a * vect.a + mat.b * vect.b,
        mat.c * vect.a + mat.d * vect.b
    )

def MultMatrix(mat1, mat2):
    return Matrix2x2(
        mat1.a * mat2.a + mat1.b * mat2.c,
        mat1.a * mat2.b + mat1.b * mat2.d,
        mat1.c * mat2.a + mat1.d * mat2.c,
        mat1.c * mat2.b + mat1.d * mat2.d
    )

def Add(v1, v2):
    return Vect2(v1.a + v2.a, v1.b + v2.b)

def CrossProduct(v1, v2):
    A = Matrix2x2(v1.b, v1.c, v2.b, v2.c)
    B = Matrix2x2(v1.a, v1.c, v2.a, v2.c)
    C = Matrix2x2(v1.a, v1.b, v2.a, v2.b)

    return Vect3(Det(A), Det(B), Det(C))

def VectsParallel(v1, v2):
    cross_product = CrossProduct(v1, v2)

    return (
        abs(cross_product.a) < ERROR_MARGIN and
        abs(cross_product.b) < ERROR_MARGIN and
        abs(cross_product.c) < ERROR_MARGIN
    )
