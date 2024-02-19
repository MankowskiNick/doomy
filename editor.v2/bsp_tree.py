from editor_math import *
from map import *

class BSP_Node:
    def __init__(self, id, front, back, walls):
        self.id = id
        self.front = front
        self.back = back
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
        self.bsp_head = self.generate_bsp_subtree(self.ref_map.Walls)

    def generate_bsp_subtree(self, walls):

        # Check if walls forms a convex subsector, if so, we are done. 
        # Return a new leaf node that indicates it is a subsector
        if (self.is_subsector(walls)):
            return BSP_Node(self._get_node_id(), None, None, [w.id for w in walls])

        # Define a list of front and back walls
        front_walls = []
        back_walls = []

        # Pick a partition wall
        partition_wall = walls[int(len(walls) / 2)]
        partition_line = extend_line(partition_wall.line)

        for wall in walls:
            if wall.id == partition_wall.id or wall.is_ancestral == 1:
                continue

            intersection = find_intersection(wall.line, partition_line)

            v1_equal = vertex_equals(intersection, wall.line.v1)
            v2_equal = vertex_equals(intersection, wall.line.v2)

            # Easy case, the wall is wholely either in front or behind the current partition wall
            if (intersection is None or v1_equal or v2_equal):
                if is_front_walls(wall, partition_wall):
                    front_walls.append(wall)
                else:
                    back_walls.append(wall)

            # We need to split the wall into a front and back portion - this still seems to not work entirely.  
            # Inspecting the output for the map, it appears to work.  But still getting some overdraw issues.
            else:
                # Add the intersection as a vertex
                new_vert = self.ref_map.AddVertex(intersection.x, intersection.y)
                new_vert.set_temp()

                # Only set a wall as ancestral if it isn't temporary
                if not wall.is_temp:
                    wall.set_ancestral()

                # Add the new wall and update v2 of the wall we are modifying
                new_wall1 = self.ref_map.AddWall(new_vert.id, 
                                                wall.line.v2.id, 
                                                wall.color, 
                                                wall.min_height, wall.max_height, 
                                                wall.floor_height, wall.ceiling_height)
                new_wall1.set_temp()

                new_wall2 = self.ref_map.AddWall(wall.line.v1.id, 
                                                new_vert.id, 
                                                wall.color, 
                                                wall.min_height, wall.max_height, 
                                                wall.floor_height, wall.ceiling_height)
                new_wall2.set_temp()

                if is_front_walls(new_wall1, partition_wall):
                    front_walls.append(new_wall1)
                    back_walls.append(new_wall2)
                else:
                    back_walls.append(new_wall1)
                    front_walls.append(new_wall2)
                
        
        front_subtree = self.generate_bsp_subtree(front_walls)
        back_subtree = self.generate_bsp_subtree(back_walls)
        return_tree = BSP_Node(self._get_node_id(), front_subtree, back_subtree, [partition_wall.id]) # This went from using partition_wall.id to new node id

        return return_tree

    def is_subsector(self, walls): # Check if walls is []?
        if len(walls) == 0:
            return True
            
        # Get the list of vertices
        vertices_unfiltered = [wall.line.v1 for wall in walls] + [wall.line.v2 for wall in walls]

        # Filter to unique vertices
        vertices = []
        [vertices.append(v) for v in vertices_unfiltered if v not in vertices]

        check_vertex = vertices[0]

        # Get the lines in the sector
        lines = [wall.line for wall in walls]
        for v in vertices:
            edge_count = 0
            for line in lines:
                if line.v1 == v or line.v2 == v:
                    edge_count += 1
            
        adjacent_lines = [line for line in lines if line.v1 == check_vertex or line.v2 == check_vertex]

        # If there are more than 2 walls containing the check vertex, we cannot have a convex sector.
        if len(adjacent_lines) < len(walls) - 2:
            return False

        # Check if drawing a line from v to check vertex intersects with another line.  If it does, the sector is not convex.
        for v in vertices:
            if v == check_vertex:
                continue
            
            check_line = Line(check_vertex, v)
            if check_line not in adjacent_lines:
                for l in lines:
                    if intersect(check_line, l):
                        return False

        return True

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

    def _get_node_id(self):
        self.max_id += 1
        return self.max_id

    # Return a list of sectors in the bsp tree
    def get_sectors(self):
        return self._get_sectors_helper(self.bsp_head)

    # Return a list of sectors in cur_head
    def _get_sectors_helper(self, cur_head):
        # If cur_head is none, there aren't any sectors in cur_head
        if cur_head is None:
            return []
        
        # Otherwise, return a list of the sectors in front of cur_head, behind cur_head, and cur_head
        return [cur_head] + self._get_sectors_helper(cur_head.front) + self._get_sectors_helper(cur_head.back)
