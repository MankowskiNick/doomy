import editor_math

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

        for wall in walls:
            if wall.id == partition_wall.id:
                continue

            intersection = editor_math.find_intersection(wall.line, partition_wall.line)

            v1_equal = editor_math.vertex_equals(intersection, wall.line.v1)
            v2_equal = editor_math.vertex_equals(intersection, wall.line.v2)

            # if (intersection is None or v1_equal or v2_equal):
            if editor_math.is_front_walls(wall, partition_wall):
                front_walls.append(wall)
            else:
                back_walls.append(wall)

            # TODO: Split walls that intersect with the dividing wall
        
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