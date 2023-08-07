import editor_math
from map import *

class BSP_Node:
    def __init__(self, id, front, back):
        self.id = id
        self.front = front
        self.back = back


class BSP_Tree:
    def __init__(self, map):
        self.ref_map = map
        self.bsp_head = None

        # Generate BSP Tree
        self.generate_bsp_tree()

    def generate_bsp_tree(self):
        # Set bsp_head to the 
        self.bsp_head = self.generate_bsp_subtree(self.ref_map.Walls)

    def generate_bsp_subtree(self, walls):

        # Nothing to partition, return None
        if len(walls) == 0:
            return None

        # Define a list of front and back walls
        front_walls = []
        back_walls = []

        # Pick a partition wall
        partition_wall = walls[int(len(walls) / 2)]
        partition_line = editor_math.extend_line(partition_wall.line)

        for wall in walls:
            if wall.id == partition_wall.id or wall.is_ancestral == 1:
                continue

            intersection = editor_math.find_intersection(wall.line, partition_line)

            v1_equal = editor_math.vertex_equals(intersection, wall.line.v1)
            v2_equal = editor_math.vertex_equals(intersection, wall.line.v2)

            # Easy case, the wall is wholely either in front or behind the current partition wall
            if (intersection is None or v1_equal or v2_equal):
                if editor_math.is_front_walls(wall, partition_wall):
                    front_walls.append(wall)
                else:
                    back_walls.append(wall)

            # We need to split the wall into a front and back portion - this still seems to not work entirely.  
            # Inspecting the output for the map, it appears to work.  But still getting some overdraw issues.
            else:
                # Add the intersection as a vertex
                new_vert_id = self.ref_map.GetNewVertexId()
                new_vert = Vertex(new_vert_id, intersection.x, intersection.y, intersection.z)
                new_vert.set_temp()
                self.ref_map.AddExistingVertex(new_vert)

                # Only set a wall as ancestral if it isn't temporary
                if not wall.is_temp:
                    wall.set_ancestral()

                # Add the new wall and update v2 of the wall we are modifying
                new_line1 = Line(new_vert, wall.line.v2)
                new_wall1 = Wall(self.ref_map.GetNewWallId(), new_line1, wall.height, wall.color)
                new_wall1.set_temp()
                self.ref_map.AddExistingWall(new_wall1)

                new_line2 = Line(wall.line.v1, new_vert)
                new_wall2 = Wall(self.ref_map.GetNewWallId(), new_line2, wall.height, wall.color)
                new_wall2.set_temp()
                self.ref_map.AddExistingWall(new_wall2)

                if editor_math.is_front_walls(new_wall1, partition_wall):
                    front_walls.append(new_wall1)
                    back_walls.append(new_wall2)
                else:
                    back_walls.append(new_wall1)
                    front_walls.append(new_wall2)
                
        
        front_subtree = self.generate_bsp_subtree(front_walls)
        back_subtree = self.generate_bsp_subtree(back_walls)
        return_tree = BSP_Node(partition_wall.id, front_subtree, back_subtree)

        return return_tree

    # Return structure:
    # (id=int, front=(), back=()), # is a special character denoting an empty child
    def to_string(self):
        return self._to_string_helper(self.bsp_head)

    def _to_string_helper(self, cur_head):
        if cur_head is None:
            return "#"

        front_string = self._to_string_helper(cur_head.front)
        back_string = self._to_string_helper(cur_head.back)

        return "(id=" + str(cur_head.id) + ",front=" + front_string + ",back=" + back_string + ")"

    def get_map(self):
        return self.ref_map