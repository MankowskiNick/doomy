class Vertex:
    def __init__(self, id, x, y, z):
        self.id = id
        self.x = x
        self.y = y
        self.z = z
        self.is_temp = 0
    def set_temp(self):
        self.is_temp = 1

class Line:
    def __init__(self, v1, v2):
        self.v1 = v1
        self.v2 = v2

class Wall:
    def __init__(self, id, line, height, color):
        self.id = id
        self.line = line
        self.height = height
        self.color = color
        self.is_temp = 0
        self.is_ancestral = 0
    def edit_wall(self, v1, v2, height, color):
        self.line = Line(v1, v2)
        self.height = height
        self.color = color
    def set_temp(self):
        self.is_temp = 1
    def set_ancestral(self):
        self.is_ancestral = 1
    

class Map:
    def __init__(self):
        self.Walls = []
        self.Vertices = []

    def AddVertex(self, id, x, y, z):
        vert = Vertex(id, x, y, z)
        self.AddExistingVertex(vert)

    def AddExistingVertex(self, vert):
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
            wall_id = len(self.Walls)
            wall = Wall(wall_id, line, wall_height, [r, g, b])
            for vertex in self.Vertices:
                if vertex.id == v1.id:
                    wall.line.v1 = vertex
                if vertex.id == v2.id:
                    wall.line.v2 = vertex
            self.AddExistingWall(wall)
        else:
            print("Failed to add wall with vertex ids:" + str(v1_id) + " " + str(v2_id))

    def AddExistingWall(self, wall):
        self.Walls.append(wall)
    
    def AddWallWithId(self, id, v1_id, v2_id, wall_height, r, g, b):
        v1, v2 = None, None
        for v in self.Vertices:
            if v1_id == v.id:
                v1 = v
            elif v2_id == v.id:
                v2 = v
        if v1 != None and v2 != None:
            line = Line(v1, v2)
            wall = Wall(id, line, wall_height, [r, g, b])
            for vertex in self.Vertices:
                if vertex.id == v1.id:
                    wall.line.v1 = vertex
                if vertex.id == v2.id:
                    wall.line.v2 = vertex
            self.Walls.append(wall)
        else:
            print("Failed to add wall with vertex ids:" + str(v1_id) + " " + str(v2_id))