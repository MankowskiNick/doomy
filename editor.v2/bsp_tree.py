from editor_math import *
from map import *

SUBSECTOR = 0x8000

class BSP_Node:
    def __init__(self, id, front, back, wall_id, is_subsector=False):
        self.id = id
        self.front = front
        self.back = back
        self.wall_id = wall_id
        self.is_subsector = is_subsector

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
        self.bsp_head = self.generate_subtree(self.ref_map.Walls, True)

    def cut_wall(self, intersection, wall):
        new_vert = self.ref_map.AddVertex(intersection.x, intersection.y)
        new_vert.set_temp()

        wall1 = self.ref_map.AddWall(
            wall.line.v1.id, new_vert.id, 
            wall.color,
            wall.min_height, wall.max_height, 
            wall.floor_height, wall.ceiling_height)
        wall1.set_temp()

        wall2 = self.ref_map.AddWall(
            new_vert.id, wall.line.v2.id, 
            wall.color, 
            wall.min_height, wall.max_height, 
            wall.floor_height, wall.ceiling_height)
        wall2.set_temp()
        return wall1, wall2

    def generate_subtree(self, walls, possible_subsector = True):
        # Base case, no walls to partition.  Return a leaf
        if len(walls) == 0:
            return None

        split_wall = walls[int(len(walls) / 2)]
        split_line = extend_line(split_wall.line)

        front_walls = []
        back_walls = []

        # Partition walls
        for w in walls:
            if w.id == split_wall.id or w in front_walls or w in back_walls or w.is_ancestral:
                continue

            intersection = find_intersection(split_line, w.line)
            v1_equals = vertex_equals(w.line.v1, intersection)
            v2_equals = vertex_equals(w.line.v2, intersection)

            # Split the wall into a front half and a back half
            if intersection is not None and not v1_equals and not v2_equals:
                w.set_ancestral()
                wall1, wall2 = self.cut_wall(intersection, w)

                if is_front_walls(wall1, split_wall):
                    front_walls.append(wall1)
                    back_walls.append(wall2)
                else:
                    back_walls.append(wall1)
                    front_walls.append(wall2)
            else:
                if is_front_walls(w, split_wall):
                    front_walls.append(w)
                else:
                    back_walls.append(w)

        front_tree = None
        back_tree = None

        if not possible_subsector:
            front_tree = self.generate_subtree(front_walls, False)
            back_tree = self.generate_subtree(back_walls, False)
        else:
            front_subsector = self.is_subsector([split_wall] + front_walls)
            back_subsector = self.is_subsector([split_wall] + back_walls)
            if front_subsector:
                self.subsectors.append(Subsector(self._get_node_id(), [split_wall] + front_walls))
            if back_subsector:
                self.subsectors.append(Subsector(self._get_node_id(), [split_wall] + back_walls))

            front_tree = self.generate_subtree(front_walls, not front_subsector)
            back_tree = self.generate_subtree(back_walls, not back_subsector)

        return_tree = BSP_Node(self._get_node_id(), front_tree, back_tree, split_wall.id, self.is_subsector(walls)) # last parameter may be cap
                
        return return_tree

    def is_subsector(self, walls):
        # Get the list of vertices
        vertices_unfiltered = [wall.line.v1 for wall in walls] + [wall.line.v2 for wall in walls]

        # Filter to unique vertices
        vertices = []
        [vertices.append(v) for v in vertices_unfiltered if v not in vertices]
        
        return is_convex_polygon(vertices)

    # Return structure:
    # (id=int, front=(), back=()), # is a special character denoting an empty child
    def to_string(self):
        return self._to_string_helper(self.bsp_head)

    def _to_string_helper(self, cur_head):
        if cur_head is None:
            return "#"

        front_string = self._to_string_helper(cur_head.front)
        back_string = self._to_string_helper(cur_head.back)

        return "(id=" + str(cur_head.id + (SUBSECTOR if cur_head.is_subsector else 0)) + ",wall_id=" + str(cur_head.wall_id) + ",front=" + front_string + ",back=" + back_string + ")"

    def get_map(self):
        return self.ref_map

    def _get_node_id(self):
        self.max_id += 1
        return self.max_id

    # Return a list of sectors in the bsp tree
    def get_sectors(self):
        return self.subsectors
