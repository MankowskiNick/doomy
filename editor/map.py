def CreateEmptyMap():
    return Map()

class Vertex:
    def __init__(self, id, x, y, z):
        self.id = id
        self.x = x
        self.y = y
        self.z = z

class Line:
    def __init__(self, v1, v2):
        self.v1 = v1
        self.v2 = v2

class Wall:
    def __init__(self, line, height, color):
        self.line = line
        self.height = height
        self.color = color

class Map:
    def __init__(self):
        self.Walls = []
        self.Vertices = []

    def AddVertex(self, id, x, y, z):
        vert = Vertex(id, x, y, z)
        self.Vertices.append(vert)

    def AddWall(self, id1, id2, wall_height, r, g, b):
        wall = Wall(None, wall_height, [r, g, b])
        for vertex in self.Vertices:
            if vertex.id == id1:
                wall.line.v1 = vertex
            if vertex.id == id2:
                wall.line.v2 = vertex
        self.Walls.append(wall)
