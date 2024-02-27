class Vertex:
    def __init__(self, id, x, y):
        self.id = id
        self.x = x
        self.y = y
        self.is_temp = 0
    def set_temp(self):
        self.is_temp = 1

class Line:
    def __init__(self, v1, v2):
        self.v1 = v1
        self.v2 = v2

class Wall:
    def __init__(self, id, line, color, min_height, max_height, floor_height = -1, ceiling_height = -1):
        self.id = id
        self.line = line
        self.color = color

        self.min_height = min_height
        self.max_height = max_height
        self.floor_height = floor_height
        self.ceiling_height = ceiling_height

        self.is_temp = 0
        self.is_ancestral = 0

    def edit_wall(self, v1, v2, color, min_height, max_height, floor_height = -1, ceiling_height = -1):
        self.line = Line(v1, v2)
        self.color = color

        self.min_height = min_height
        self.max_height = max_height
        self.floor_height = floor_height
        self.ceiling_height = ceiling_height

    def set_temp(self):
        self.is_temp = 1
    def set_ancestral(self):
        self.is_ancestral = 1
    
class Map:
    def __init__(self):
        self.Walls = []
        self.Vertices = []
        
    def AddVertex(self, x, y):
        return self.AddVertexWithId(self.GetNewVertexId(), x, y)

    def AddVertexWithId(self, id, x, y):
        vert = Vertex(id, x, y)
        self.Vertices.append(vert)
        return vert


    def AddWall(self, v1_id, v2_id, color, min_height, max_height, floor_height = -1, ceiling_height = -1):
        return self.AddWallWithId(self.GetNewWallId(),
                                v1_id,
                                v2_id,
                                color,
                                min_height, max_height,
                                floor_height, ceiling_height)

    def AddWallWithId(self, id ,v1_id, v2_id, color, min_height, max_height, floor_height = -1, ceiling_height = -1):
        v1, v2 = None, None
        for v in self.Vertices:
            if v1_id == v.id:
                v1 = v
            elif v2_id == v.id:
                v2 = v
        if v1 != None and v2 != None:
            line = Line(v1, v2)
            wall_id = id
            wall = Wall(wall_id, line, color, min_height, max_height, floor_height, ceiling_height)
            for vertex in self.Vertices:
                if vertex.id == v1.id:
                    wall.line.v1 = vertex
                if vertex.id == v2.id:
                    wall.line.v2 = vertex
            self.Walls.append(wall)
            return wall
        else:
            print("Failed to add wall with vertex ids:" + str(v1_id) + " " + str(v2_id))
            return None

    def GetNewWallId(self):
        ids = [w.id for w in self.Walls]
        return self._GetNewId(ids)    
    
    def GetNewVertexId(self):
        ids = [v.id for v in self.Vertices]
        return self._GetNewId(ids)

    def GetWallById(self, id):
        for w in self.Walls:
            if w.id == id:
                return w
        return None

    def _GetNewId(self, ids):
        new_id = 0
        while new_id in ids:
            new_id += 1
        return new_id

    def GetFilteredMap(self):
        new_map = Map()
        new_map.Walls = [w for w in self.Walls if not w.is_temp]
        new_map.Vertices = [v for v in self.Vertices if not v.is_temp]
        return new_map