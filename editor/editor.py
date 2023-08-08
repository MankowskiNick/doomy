import tkinter as tk
import math
from tkinter import messagebox
from tkinter import filedialog

from bsp_tree import BSP_Tree
from wall_dialog import WallDialog
from vertex_dialog import VertexDialog
from map import *
from editor_math import *

# TODO:
#   -Build in support for attaching images
#   -Fix bug with line selection not working in add wall mode
#   -Bug where splitting walls cause random artifacts(Maybe we should make the walls/vertices added in this process temporary?)
#   -Normalize xy and z values when loading/saving maps

def rgb_to_hex(rgb):
    return '#%02x%02x%02x' % tuple(rgb)    

class MapEditorGUI:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Map Editor")
        self.canvas_width = 1000
        self.canvas_height = 750
        self.canvas = tk.Canvas(self.root, width=self.canvas_width, height=self.canvas_height, bg="white")
        self.canvas.pack()

        self.map = Map()
        self.selected_vertex = None
        self.selected_wall = None
        self.drag_vert_id = None
        self.dragging = False

        self.textures = []

        self.canvas.bind("<Button-1>", self.on_canvas_click)
        self.canvas.bind("<B1-Motion>", self.drag)
        self.canvas.bind("<ButtonRelease-1>", self.drag_stop)

        self.mapping_scalar = 0.05

        self.create_menu()
        self.configure_buttons()
        self.root.mainloop()

    def create_menu(self):
        menubar = tk.Menu(self.root)
        file_menu = tk.Menu(menubar, tearoff=0)
        file_menu.add_command(label="New Map", command=self.new_map)
        file_menu.add_command(label="Save Map", command=self.save_map)
        file_menu.add_command(label="Load Map", command=self.load_map)
        menubar.add_cascade(label="File", menu=file_menu)

        edit_menu = tk.Menu(menubar, tearoff=0)
        edit_menu.add_command(label="Delete Vertex", command=self.delete_vertex)
        edit_menu.add_command(label="Delete Wall", command=self.delete_wall)
        menubar.add_cascade(label="Edit", menu=edit_menu)

        self.root.config(menu=menubar)

    def configure_buttons(self):        
        self.button_frame = tk.Frame(self.root)  # Create a frame for the buttons
        self.button_frame.pack()  # Pack the frame into the window

        self.is_vertex_mode = True
        self.vert_mode_button = tk.Button(self.button_frame, text = "Mode: " + "Add Vertices" if self.is_vertex_mode else "Add Walls", command=self.swap_vertex_mode)
        self.edit_vert_button = tk.Button(self.button_frame, text = "Edit Vertex", command=self.edit_vertex)
        self.delete_vert_button = tk.Button(self.button_frame, text = "Delete Vertex", command=self.delete_vertex)
        self.delete_wall_button = tk.Button(self.button_frame, text = "Delete Wall", command=self.delete_wall)
        self.edit_wall_button = tk.Button(self.button_frame, text = "Edit Wall", command=self.edit_wall)

        self.vert_mode_button.pack(side="left")
        self.edit_vert_button.pack(side="left")
        self.delete_vert_button.pack(side="left")
        self.edit_wall_button.pack(side="left")
        self.delete_wall_button.pack(side="left")

        # Pack the buttons into the frame with center alignment
        self.button_frame.pack(anchor="center")

    def on_canvas_click(self, event):

        # Don't worry about processing clicks if we are dragging
        if self.dragging:
            return

        # Get the x & y of the event
        x, y = event.x, event.y

        # If we are adding vertices rather than walls,
        if (self.is_vertex_mode):

            # If we successfully select a vertex, redraw the canvas and exit
            if self.select_vertex(event):
                self.redraw_canvas()
                return

            # If we successfully select a wall, redraw the canvas and exit
            elif self.select_wall(event):
                self.redraw_canvas()
                return

            # Otherwise, create a vertex
            else:
                self.create_vertex(x, y)

        # If we are in wall creation mode,
        else:

            # Get the current vertex
            cur_vertex = self.get_vertex(x, y)

            # If there is no selected vertex,
            if self.selected_vertex is None:
                # Try to select one and redraw the canvas
                self.select_vertex(event)
                self.redraw_canvas()
                return

            # If it is a valid vertex and it's not the same one as the selected one, 
            if (cur_vertex != None and cur_vertex.id != self.selected_vertex.id):
                # Create a wall from it to the selected one
                self.create_wall(cur_vertex, self.selected_vertex)

            # If it is the same vertex,
            elif (cur_vertex.id == self.selected_vertex.id):
                # Deselect it
                self.selected_vertex = None

            if (cur_vertex == None):
                self.select_wall(event)
                self.redraw_canvas()
                return
        # Redraw the canvas, just to be safe
        self.redraw_canvas()

    def drag(self, event):
        # Get the x and y of the event
        x, y = event.x, event.y

        # If not dragging,
        if not self.dragging:
            # Start
            self.dragging = True

            # Get the id of the vertex we are dragging and store it
            drag_vert = self.get_vertex(x, y)
            if drag_vert != None:
                self.drag_vert_id = drag_vert.id

        # Get the vertex
        vert = self.get_vertex_by_id(self.drag_vert_id)

        # If we have a vertex, modify it to go to x, y
        if vert != None:
            self.modify_vertex(self.drag_vert_id, x, y, vert.z)

    def drag_stop(self, event):
        # Stop dragging
        self.dragging = False

    def create_vertex(self, x, y):
        vertex_dialog = VertexDialog(self.root, x, y, 0, self.build_vertex)

    def build_vertex(self, x, y, z):
        self.map.AddVertex(x, y, z)
        self.redraw_canvas()

    def edit_vertex(self):
        vert = self.selected_vertex
        if (vert != None):
            vertex_dialog = VertexDialog(self.root, vert.id, vert.x, vert.y, vert.z, self.modify_vertex)
        else:
            messagebox.showinfo("Error", "Please select a vertex first.")
        
    def modify_vertex(self, id, x, y, z):

        # Modify vertex
        vert = self.get_vertex_by_id(id)
        self.map.Vertices.remove(vert)
        self.map.AddVertex(id, x, y, z)

        vert.x = x
        vert.y = y
        vert.z = z

        # Modify walls attached
        for wall in self.map.Walls:
            if (wall.line.v1.id == id):
                wall.line.v1 = vert
            if (wall.line.v2.id == id):
                wall.line.v2 = vert

        self.selected_vertex = vert

        self.redraw_canvas()

    def create_wall(self, v1, v2):
        if (v1.z == v2.z):
            wall_dialog = WallDialog(self.root, v1, v2, 1, 255, 255, 255, self.build_wall)
        else:
            messagebox.showinfo("Error", "Vertices must be at same z level to create a wall.")
    
    def build_wall(self, v1, v2, height, color):
        self.map.AddWall(v1.id, v2.id, height, *color)
        self.redraw_canvas()

    def select_vertex(self, event):
        x = event.x
        y = event.y

        # Iterate over the vertices and check if the click coordinates are within a certain range
        potential_vertex = self.get_vertex(x, y)

        if potential_vertex != None:
            selected_vertex_id = -1 if self.selected_vertex == None else self.selected_vertex.id
            if (potential_vertex.id == selected_vertex_id):
                self.selected_vertex = None
            else:
                self.selected_vertex = potential_vertex
                self.selected_wall = None
        return potential_vertex != None

    def get_vertex(self, x, y):
        for vertex in self.map.Vertices:
            if abs(vertex.x - x) <= 5 and abs(vertex.y - y) <= 5:
                # Return the vertex
                return vertex
        return None

    def swap_vertex_mode(self):
        self.is_vertex_mode = False if self.is_vertex_mode else True
        self.vert_mode_button.configure(text="Mode: " + ("Add Vertices" if self.is_vertex_mode else "Add Walls"))

    def select_wall(self, event):
        x = event.x
        y = event.y

        # Iterate over the walls and check if the click coordinates are near any wall
        for wall in self.map.Walls:
            if self.is_near_wall(x, y, wall):
                selected_wall = self.selected_wall
                if selected_wall == wall:
                    self.selected_wall = None
                else:
                    self.selected_wall = wall
                    self.selected_vertex = None
                return True

        return False

    def is_near_wall(self, x, y, wall):
        # Calculate the distance between the click coordinates and the wall line segment
        x1, y1 = wall.line.v1.x, wall.line.v1.y
        x2, y2 = wall.line.v2.x, wall.line.v2.y

        threshhold = 5

        # Create a line that is perpendicular to the line we are checking that has a length of threshold * 2
        angle = math.atan2(x1 - x2, y2 - y1)
        check_v1 = Vertex(-1, x - threshhold * math.cos(angle), y - (threshhold * math.sin(angle)), 0)
        check_v2 = Vertex(-1, x + threshhold * math.cos(angle), y + (threshhold * math.sin(angle)), 0)
        perp_line = Line(check_v1, check_v2)

        # Return true if this intersects with wall, otherwise return false
        return intersect(perp_line, wall.line)

    def delete_vertex(self):
        if self.selected_vertex:
            vertex_id = self.selected_vertex.id
            self.map.Vertices = [v for v in self.map.Vertices if v.id != vertex_id]
            self.map.Walls = [w for w in self.map.Walls if w.line.v1.id != vertex_id and w.line.v2.id != vertex_id]
            self.selected_vertex = None
            self.redraw_canvas()

    def delete_wall(self):
        if self.selected_wall:
            self.map.Walls.remove(self.selected_wall)
            self.selected_wall = None
            self.redraw_canvas()

    def edit_wall(self):
        wall = self.selected_wall
        if (wall != None):
            r, g, b = wall.color[0], wall.color[1], wall.color[2]
            wall_dialog = WallDialog(self.root, wall.line.v1, wall.line.v2, wall.height, r, g, b, self.modify_wall, wall.id)
        else:
            messagebox.showinfo("Error", "Please select a wall first.")

    def modify_wall(self, v1, v2, height, color):
        # Edit the current wall
        self.selected_wall.edit_wall(v1, v2, height, color)

        # Redraw canvas
        self.redraw_canvas()
    
    def redraw_canvas(self):
        self.canvas.delete("all")
        for vertex in self.map.Vertices:
            x, y = vertex.x, vertex.y
            if (self.selected_vertex != None and self.selected_vertex.id == vertex.id):
                self.canvas.create_oval(x - 7, y - 7, x + 7, y + 7, fill="red")
            self.canvas.create_oval(x - 5, y - 5, x + 5, y + 5, fill="black")
            self.canvas.create_text(x + 10, y - 10, text=str(vertex.id) + ": (" + str(vertex.x) + " , " + str(vertex.y) + ")", fill="black", font=('Arial', 12))

        for wall in self.map.Walls:
            x1, y1 = wall.line.v1.x, wall.line.v1.y
            x2, y2 = wall.line.v2.x, wall.line.v2.y
            color = rgb_to_hex(wall.color)


            if (self.selected_wall != None and self.selected_wall == wall):
                self.canvas.create_line(x1 - 1, y1 + 1, x2 - 1, y2 + 1, fill=color)
                self.canvas.create_line(x1 + 1, y1 - 1, x2 + 1, y2 - 1, fill=color)
                self.canvas.create_line(x1 + 1, y1 + 1, x2 + 1, y2 + 1, fill=color)
                self.canvas.create_line(x1 - 1, y1 - 1, x2 - 1, y2 - 1, fill=color)
            self.canvas.create_line(x1, y1, x2, y2, fill=color)

    def get_vertex_by_id(self, vertex_id):
        for vertex in self.map.Vertices:
            if vertex.id == vertex_id:
                return vertex
        return None

    def get_wall_by_id(self, wall_id):
        for wall in self.map.Walls:
            if wall.id == wall_id:
                return wall
        return None

    def new_map(self):
        self.map = map.Map()
        self.canvas.delete("all")

    def save_map(self):
        # File format: 
        #   [verts]
        #   v1.id v1.x v1.y v1.z is_temp
        #   ...
        #   [walls]
        #   id v1.id v2.id height r g b is_temp is_ancestral
        #   ...
        #   [bsp]
        #   bsp string
        #   [end]

        # Process the bsp for the map and reassign self.map to be this
        bsp = BSP_Tree(self.map)
        self.map = bsp.get_map()

        contents = ""

        contents += "[verts]\n"
        for v in self.map.Vertices:
            x, y, z = self.map_coords_to_file(v.x, v.y, v.z)
            contents += "vert: " + str(v.id) + " " + str(x) + " " + str(y) + " " + str(z) + " " + str(v.is_temp) + "\n"
        
        contents += "[walls]\n"
        for w in self.map.Walls:
            vert1_id = str(w.line.v1.id) # TODO: why does reversing these change the draw order?
            vert2_id = str(w.line.v2.id)

            contents += "wall: "  + str(w.id) + " " + vert1_id + " " + vert2_id + " " + str(w.height) + " " + str(w.color[0]) + " " + str(w.color[1]) + " " + str(w.color[2]) +  " " + str(w.is_temp) + " " + str(w.is_ancestral) + "\n"
        
        
        contents += "[bsp]\n"

        contents += bsp.to_string() + "\n"

        contents += "[end]"

        file_path = filedialog.asksaveasfilename(defaultextension=".dat")
        if file_path:
            with open(file_path, "w") as file:
                file.write(contents)

    def map_coords_to_file(self, x, y, z):
        x = (x - (self.canvas_width / 2)) * self.mapping_scalar
        y = (y - (self.canvas_height / 2)) * self.mapping_scalar
        z = z # * self.mapping_scalar
        return x, y, z

    def map_coords_from_file(self, x, y, z):
        x = (x / self.mapping_scalar) + (self.canvas_width / 2)
        y = (y / self.mapping_scalar) + (self.canvas_height / 2)
        z = z
        return x, y, z

    def load_map(self):
        file_path = filedialog.askopenfilename(filetypes=[("Map Files", "*.dat")])
        if file_path:
            with open(file_path, 'r') as file:
                self.map = Map()
                section = None
                for line in file:
                    line = line.strip()
                    if line == '[verts]':
                        section = 'verts'
                    elif line == '[walls]':
                        section = 'walls'
                    elif line == '[end]':
                        section = None
                    else:
                        if section == 'verts':
                            self.parse_vertex(line)
                        elif section == 'walls':
                            self.parse_wall(line)
            self.redraw_canvas()

    def parse_vertex(self, line):
        if line.startswith('vert:'):
            parts = line.split()
            if len(parts) == 6:
                id = int(parts[1])
                x, y, z = float(parts[2]), float(parts[3]), float(parts[4])
                x, y, z = self.map_coords_from_file(x, y, z)
                is_temp = int(parts[5])
                if is_temp == 0:
                    self.map.AddVertexWithId(id, x, y, z)
            else:
                messagebox.showinfo("Error parsing file.")

    def parse_wall(self, line):
        if line.startswith('wall:'):
            parts = line.split()
            if len(parts) == 10:
                wall_id = int(parts[1])
                v1_id = int(parts[2])
                v2_id = int(parts[3])
                wall_height = float(parts[4])
                r = int(parts[5])
                g = int(parts[6])
                b = int(parts[7])
                is_temp = int(parts[8])
                if is_temp == 0:
                    self.map.AddWallWithId(wall_id, v1_id, v2_id, wall_height, r, g, b)
            else:
                messagebox.showinfo("Error parsing file.")

    def add_texture(self):
        pass

if __name__ == "__main__":
    MapEditorGUI()