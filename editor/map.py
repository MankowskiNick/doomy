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

    def AddWall(self, v1_id, v2_id, wall_height, r, g, b):
        v1, v2 = None, None
        for v in self.Vertices:
            if v1_id == v.id:
                v1 = v
            elif v2_id == v.id:
                v2 = v
        if v1 != None and v2 != None:
            line = Line(v1, v2)
            wall = Wall(line, wall_height, [r, g, b])
            for vertex in self.Vertices:
                if vertex.id == v1.id:
                    wall.line.v1 = vertex
                if vertex.id == v2.id:
                    wall.line.v2 = vertex
            self.Walls.append(wall)
        else:
            print("Failed to add wall with vertex ids:" + str(v1_id) + " " + str(v2_id))
