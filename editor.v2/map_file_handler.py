#from map import *
from bsp_tree import *
from editor_math import *

class MapFileHandler:
    def __init__(self, width, height, mappingScalar):
        # File format: 
        #   [verts]
        #   v1.id v1.x v1.y is_temp
        #   ...
        #   [walls]
        #   id v1.id v2.id wall_min wall_max floor_min floor_max r g b is_temp is_ancestral
        #   ...
        #   [sectors]   # sectors are bsp nodes, not necessarily all subsectors.  subsectors are convex sectors and leaves in the tree
        #   id wall1.id wall2.id ...
        #   [bsp]
        #   bsp string
        #   [end]
        self._MappingScalar = mappingScalar
        self._Width = width
        self._Height = height
        self.Map = Map()
        return

    def SaveMap(self, bsp, filename="lvldata.dat"):
        
        if bsp is None:
            print("Failed to save map, you must process sectors before saving.")
            return

        self.Map = bsp.get_map()

        contents = ""

        # Add vertices to file
        contents += self._GetVertexString()
                
        # Add walls to file
        contents += self._GetWallString()

        # Add sectors(bsp nodes) to file
        contents += self._GetSectorsString(bsp)
                
        # Add BSP string to file
        contents += "[bsp]\n"
        contents += bsp.to_string() + "\n"

        # End file
        contents += "[end]"

        # Open the file in write mode and save the contents
        with open("lvl/" + filename, 'w') as file:
            file.write(contents)
        print(f'Map saved to {filename}.')

    def LoadMap(self, fileName):
        with open("lvl/" + fileName, 'r') as file:
            section = None
            for line in file:
                line = line.strip()
                if line == '[verts]':
                    section = 'verts'
                elif line == '[walls]':
                    section = 'walls'
                elif line == '[sectors]':
                    section = None
                else:
                    self._HandleLine(section, line)
        return self.Map
    
    def _HandleLine(self, section, text):
        if section == 'verts':
            self._ParseVertex(text)
        elif section == 'walls':
            self._ParseWall(text)

    def _ParseVertex(self, text):
        if text.startswith('vert:'):
            parts = text.split()
            if len(parts) == 5:
                id = int(parts[1])
                x, y = float(parts[2]), float(parts[3])
                x, y = map_coords_from_file(self._Width, self._Height, self._MappingScalar, x, y)
                is_temp = int(parts[4])
                if is_temp == 0:
                    self.Map.AddVertexWithId(id, x, y)
            else:
                print("Error parsing file.")

    def _ParseWall(self, text):
        if text.startswith('wall:'):
            parts = text.split()
            if len(parts) == 13:
                wall_id = int(parts[1])
                v1_id = int(parts[2])
                v2_id = int(parts[3])
                wall_min = float(parts[4])
                wall_max = float(parts[5])
                floor_height = float(parts[6])
                ceiling_height = float(parts[7])
                color = (int(parts[8]), int(parts[9]), int(parts[10]))
                is_temp = int(parts[11])
                if is_temp == 0:
                    self.Map.AddWallWithId(wall_id, 
                                            v1_id, 
                                            v2_id, 
                                            color, 
                                            wall_min, wall_max, 
                                            floor_height, ceiling_height)
            else:
                print("Error parsing file.")

    #   [verts]
    #   v1.id v1.x v1.y is_temp
    #   ...
    def _GetVertexString(self):
        contents = "[verts]\n"
        for v in self.Map.Vertices:
            x, y = map_coords_to_file(self._Width, self._Height, self._MappingScalar, v.x, v.y)
            contents += "vert: " + str(v.id) + " " + str(x) + " " + str(y) + " " + str(v.is_temp) + "\n"
        return contents

    #   [walls]
    #   id v1.id v2.id wall_min wall_max floor_min floor_max r g b is_temp is_ancestral
    #   ...
    def _GetWallString(self):
        contents = "[walls]\n"
        for w in self.Map.Walls:
            vert1_id = str(w.line.v1.id) 
            vert2_id = str(w.line.v2.id)

            contents += "wall: "  + str(w.id) + " " + vert1_id + " " + vert2_id + " " + str(w.min_height) + " " + str(w.max_height) + " " + str(w.floor_height) + " " + str(w.ceiling_height) + " " + str(w.color[0]) + " " + str(w.color[1]) + " " + str(w.color[2]) +  " " + str(w.is_temp) + " " + str(w.is_ancestral) + "\n"
        return contents
    
    #   [sectors]   # sectors are bsp nodes, not necessarily all subsectors.  subsectors are convex sectors and leaves in the tree
    #   id wall1.id wall2.id ...
    #   ...
    def _GetSectorsString(self, bsp):
        contents = "[sectors]\n"
        sectors = bsp.get_sectors()
        for s in sectors:
            # Get the wall ids as a string
            wall_ids_string = ' '.join([str(wall.id) for wall in s.walls])
            # [wall_ids_string]
            contents += "sector: " + str(s.node_id) + " " + wall_ids_string + "\n"
        return contents