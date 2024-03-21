from editor_math import *
from map import *

SUBSECTOR = 0x8000
MAX_INT = 2 ** 32

class DivLine:
    def __init__(self, vert, dx, dy):
        self.vert = vert
        self.dx = dx
        self.dy = dy

class BSP_Node:
    def __init__(self, id, wall_id, front, back,  walls):
        self.id = id
        self.front = front
        self.back = back
        self.wall_id = wall_id if wall_id is not None else -1
        self.walls = walls

class Subsector:
    def __init__(self, node_id, walls):
        self.node_id = node_id
        self.walls = walls

class BSP_Tree:
    def __init__(self, map):
        self.ref_map = map
        self.bsp_head = None
        self.max_id = 0

        # Generate BSP Tree
        self.generate_bsp_tree()

    def generate_bsp_tree(self):
        # Set bsp_head to the 
        self.subsectors = []
        self.bsp_head = self.build_bsp_node(self.ref_map.Walls)

    def split_wall(self, div_line, wall):
        intersection = self.get_intersection(div_line, wall.line)

        new_vert = self.ref_map.AddVertex(intersection.x, intersection.y)

        v2_id = wall.line.v2.id
        wall.line.v2 = new_vert

        wall2 = self.ref_map.AddWall(
            new_vert.id, v2_id, 
            wall.color, 
            wall.min_height, wall.max_height, 
            wall.floor_height, wall.ceiling_height)

        # Return in front, back order
        if self.line_on_side(div_line, wall.line) == 0:
            return wall, wall2
        else:
            return wall2, wall

    def evaluate_split(self, split_wall, walls, best_grade):
        split_line = self.get_div_line_from_line(split_wall.line)

        front_count, back_count = 0,0

        for w in walls:
            if w.id == split_wall.id:
                continue
            else:
                side = self.line_on_side(split_line, w.line)
            
            if side == 0:
                front_count += 1
            elif side == 1:
                back_count += 1
            elif side != -1:
                front_count += 1
                back_count += 1

            max_count = max(front_count, back_count)
            new_count = front_count + back_count - len(walls)
            grade = max_count + new_count * 8 # ??? This comes from DoomBSP, why the 8?

            # Get out early, no point in continuing
            if grade > best_grade:
                return grade
        if front_count == 0 or back_count == 0:
            return MAX_INT
        
        return grade

    def pick_split_wall(self, walls):
        best_score = MAX_INT
        best_wall = None

        for w in walls:
            score = self.evaluate_split(w, walls, best_score)
            if score < best_score:
                best_score = score
                best_wall = w
        return best_wall, best_score

    def execute_split(self, split_wall, walls):
        split_line = DivLine(split_wall.line.v1, 
                                split_wall.line.v1.x - split_wall.line.v2.x, 
                                split_wall.line.v1.y - split_wall.line.v2.y)

        front_walls = []
        back_walls = []

        for i, w in enumerate(walls):
            if w.id == split_wall.id:
                side = -1
            else:
                side = self.line_on_side(split_line, w.line)

            if side == -1:  # Coincident walls should probably go in both front and back
                front_walls.append(w)
                back_walls.append(w)
            elif side == 0:
                front_walls.append(w)
            elif side == 1:
                back_walls.append(w)
            else:
                wall1, wall2 = self.split_wall(split_line, w)
                front_walls.append(wall1)
                back_walls.append(wall2)
        
        return front_walls, back_walls
        
    # -1 => Coincident
    # -2 => Intersecting
    #  0 => Front
    #  1 => Behind
    def line_on_side(self, div_line, line):
        v1_side = self.vert_on_side(div_line, line.v1)
        v2_side = self.vert_on_side(div_line, line.v2)

        # Coincident
        if v1_side == -2 and v2_side == -2:
            return -1
        
        # Both on same side
        elif v1_side == v2_side:
            return v1_side
        
        # Intersecting at an edge
        elif v1_side == -2:
            return v2_side
        elif v2_side == -2:
            return v1_side

        # Intersecting in the center
        else:
            return -2

    # -2 => Point lies on line
    #  0 => Point is in front of line
    #  1 => Point lies behind line
    def vert_on_side(self, div_line, v):
        # Get normal vector of div_line
        n_dx, n_dy = -1 * div_line.dy, div_line.dx

        # Get vector from div_line.vert to v
        dv_dx, dv_dy = div_line.vert.x - v.x, div_line.vert.y - v.y

        dot = dv_dx * n_dx + dv_dy * n_dy

        if dot == 0:
            return -2
        elif dot > 0:
            return 1
        else:
            return 0

    def get_intersection(self, div_line, line):
        # Solving Ax = B for x => x = A^-1 B
        A = Matrix2x2(  -1 * div_line.dx, line.v2.x - line.v1.x,
                        -1 * div_line.dy, line.v2.y - line.v1.y)
        
        B = Vect2(  div_line.vert.x - line.v1.x,
                    div_line.vert.y - line.v1.y)

        # If det(A) == 0, lines are coincident, so just return v1
        if Det(A) == 0:
            return line.v1 

        x = Mult(Inverse(A), B) # This x vector contains our [t, s]
        t = x.a

        # Calculate intersection based on parameterized line
        intersect = Vertex(-1,  div_line.vert.x + (div_line.dx * t), 
                                div_line.vert.y + (div_line.dy * t))

        return intersect

    def get_div_line_from_line(self, line):
        return DivLine(line.v1, 
                        line.v2.x - line.v1.x, 
                        line.v2.y - line.v1.y)

    def build_bsp_node(self, walls):

        # Get the split wall
        split_wall, wall_score = self.pick_split_wall(walls)

        # Base case: wall_score is MAX_INT => no wall is suitable for partitioning => set of walls is convex
        if wall_score == MAX_INT:
            self.subsectors.append(Subsector(self._get_node_id() + SUBSECTOR, walls))
            return BSP_Node(self.subsectors[-1].node_id, None, None, None, walls)

        front_walls, back_walls = self.execute_split(split_wall, walls)

        return_node = BSP_Node(self._get_node_id(), split_wall.id, None, None, [])
        return_node.front = self.build_bsp_node(front_walls)
        return_node.back = self.build_bsp_node(back_walls)

        return return_node

    # Return structure:
    # (id=int, front=(), back=()), # is a special character denoting an empty child
    def to_string(self):
        return self._to_string_helper(self.bsp_head)

    def _to_string_helper(self, cur_head):
        if cur_head is None:
            return "#"

        front_string = self._to_string_helper(cur_head.front)
        back_string = self._to_string_helper(cur_head.back)

        return "(id=" + str(cur_head.id) + ",wall_id=" + str(cur_head.wall_id) + ",front=" + front_string + ",back=" + back_string + ")"

    def get_map(self):
        return self.ref_map

    def _get_node_id(self):
        self.max_id += 1
        return self.max_id

    # Return a list of sectors in the bsp tree
    def get_sectors(self):
        return self.subsectors
