import tkinter as tk
import map
import load_library
import bsp_tree
import math
from tkinter import messagebox
from tkinter import filedialog

# TODO:
#   -Don't allow intersections of walls

def rgb_to_hex(rgb):
    return '#%02x%02x%02x' % tuple(rgb)    

def ccw(v1, v2, v3):
    return (v3.y - v1.y) * (v2.x - v1.x) > (v2.y - v1.y) * (v3.x - v1.x)

def Intersect(l1, l2):
    return ccw(l1.v1, l2.v1, l2.v2) != ccw(l1.v2, l2.v1, l2.v2) and ccw(l1.v1, l1.v2, l2.v1) != ccw(l1.v1, l1.v2, l2.v2)

class MapEditorGUI:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Map Editor")
        self.shared_graphics = load_library.LoadSharedGraphics()
        self.canvas_width = self.shared_graphics.GetWidth()
        self.canvas_height = self.shared_graphics.GetHeight()
        self.canvas = tk.Canvas(self.root, width=self.canvas_width, height=self.canvas_height, bg="white")
        self.canvas.pack()

        self.map = map.CreateEmptyMap()
        self.selected_vertex = None
        self.selected_wall = None
        self.drag_vert_id = None
        self.dragging = False

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
            elif (cur_vertex == None or cur_vertex.id == self.selected_vertex.id):
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
        vertex_id = len(self.map.Vertices)
        vertex_dialog = VertexDialog(self.root, vertex_id, x, y, 0, self.build_vertex)

    def build_vertex(self, id, x, y, z):
        self.map.AddVertex(id, x, y, z)
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
        # # Calculate the distance between the click coordinates and the wall line segment
        x1, y1 = wall.line.v1.x, wall.line.v1.y
        x2, y2 = wall.line.v2.x, wall.line.v2.y

        threshhold = 5

        # Create a line that is perpendicular to the line we are checking that has a length of threshold * 2
        angle = math.atan2(x1 - x2, y2 - y1)
        check_v1 = map.Vertex(-1, x - threshhold * math.cos(angle), y - (threshhold * math.sin(angle)), 0)
        check_v2 = map.Vertex(-1, x + threshhold * math.cos(angle), y + (threshhold * math.sin(angle)), 0)
        perp_line = map.Line(check_v1, check_v2)

        # Return true if this intersects with wall, otherwise return false
        return Intersect(perp_line, wall.line)

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
            self.canvas.create_text(x + 10, y - 10, text=str(vertex.id) + ": (" + str(vertex.x) + " , " + str(vertex.y) + ")")

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
        self.map = map.CreateEmptyMap()
        self.canvas.delete("all")

    def save_map(self):
        # File format: 
        #   [verts]
        #   v1.id v1.x v1.y v1.z
        #   ...
        #   [walls]
        #   id v1.id v2.id height r g b
        #   ...
        #   [bsp]
        #   bsp string
        #   [end]

        # Process the bsp for the map and reassign self.map to be this
        bsp = bsp_tree.BSP_Tree(self.map)
        self.map = bsp.get_map()

        contents = ""

        contents += "[verts]\n"
        for v in self.map.Vertices:
            x, y, z = self.map_coords_to_file(v.x, v.y, v.z)
            contents += "vert: " + str(v.id) + " " + str(x) + " " + str(y) + " " + str(z) + "\n"
        
        contents += "[walls]\n"
        for w in self.map.Walls:
            # Note: We are writing walls in order of id, so they don't need to be stored.
            # The order they are presented in the file does matter, however.  We need to normalize this or we will encounter 
            # overdraw errors with the bsp tree.  Let's do this in order of x ascending then y ascending.
            if abs(w.line.v1.x) < abs(w.line.v2.x):
                vert1_id = str(w.line.v1.id)
                vert2_id = str(w.line.v2.id)
            elif abs(w.line.v1.x) == abs(w.line.v2.x):
                if abs(w.line.v1.y) < abs(w.line.v2.y):
                    vert1_id = str(w.line.v1.id)
                    vert2_id = str(w.line.v2.id)
                else:
                    vert1_id = str(w.line.v2.id)
                    vert2_id = str(w.line.v1.id)
            else:
                vert1_id = str(w.line.v2.id)
                vert2_id = str(w.line.v1.id)

            contents += "wall: "  + str(w.id) + " " + vert1_id + " " + vert2_id + " " + str(w.height) + " " + str(w.color[0]) + " " + str(w.color[1]) + " " + str(w.color[2]) + "\n"
        
        
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
        z = z * self.mapping_scalar
        return x, y, z

    def map_coords_from_file(self, x, y, z):
        x = (x / self.mapping_scalar) + (self.canvas_width / 2)
        y = (y / self.mapping_scalar) + (self.canvas_height / 2)
        z = (z / self.mapping_scalar)
        return x, y, z

    def load_map(self):
        file_path = filedialog.askopenfilename(filetypes=[("Map Files", "*.dat")])
        if file_path:
            with open(file_path, 'r') as file:
                self.map = map.CreateEmptyMap()
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
            if len(parts) == 5:
                id = int(parts[1])
                x, y, z = float(parts[2]), float(parts[3]), float(parts[4])
                x, y, z = self.map_coords_from_file(x, y, z)
                self.map.AddVertex(id, x, y, z)
            else:
                messagebox.showinfo("Error parsing file.")

    def parse_wall(self, line):
        if line.startswith('wall:'):
            parts = line.split()
            if len(parts) == 8:
                wall_id = int(parts[1])
                v1_id = int(parts[2])
                v2_id = int(parts[3])
                wall_height = float(parts[4])
                r = int(parts[5])
                g = int(parts[6])
                b = int(parts[7])
                self.map.AddWallWithId(wall_id, v1_id, v2_id, wall_height, r, g, b)
            else:
                messagebox.showinfo("Error parsing file.")

class WallDialog:
    def __init__(self, parent, v1, v2, height, r, g, b, callback, id = None):
        self.result = None

        self.dialog = tk.Toplevel(parent)
        self.dialog.title("Add Wall" + str(id))

        self.label_height = tk.Label(self.dialog, text="Wall Height:")
        self.entry_height = tk.Entry(self.dialog)
        self.label_height.pack()
        self.entry_height.pack()

        self.label_color = tk.Label(self.dialog, text="Wall Color (R, G, B):")
        self.entry_r = tk.Entry(self.dialog)
        self.entry_g = tk.Entry(self.dialog)
        self.entry_b = tk.Entry(self.dialog)
        self.label_color.pack()
        self.entry_r.pack()
        self.entry_g.pack()
        self.entry_b.pack()

        self.button_ok = tk.Button(self.dialog, text="OK", command=self.ok)
        self.button_cancel = tk.Button(self.dialog, text="Cancel", command=self.cancel)
        self.button_ok.pack()
        self.button_cancel.pack()

        self.postdialog_callback = callback
        self.v1 = v1
        self.v2 = v2

        self.height = height
        self.color = [r, g, b]
        
        self.entry_r.insert(0, str(r))
        self.entry_g.insert(0, str(g))
        self.entry_b.insert(0, str(b))
        self.entry_height.insert(0, str(height))

    def ok(self):
        try:
            wall_height = float(self.entry_height.get())
            wall_color = [int(self.entry_r.get()), int(self.entry_g.get()), int(self.entry_b.get())]
            self.result = True
            self.wall_height = wall_height
            self.wall_color = wall_color
            self.dialog.destroy()

            self.postdialog_callback(self.v1, self.v2, self.wall_height, self.wall_color)
        except ValueError:
            messagebox.showinfo("Error", "Invalid input.")

    def cancel(self):
        self.result = False
        self.dialog.destroy()

class VertexDialog:
    def __init__(self, parent, vertex_id, x, y, z, callback):
        self.result = False

        self.dialog = tk.Toplevel(parent)
        self.dialog.title("Add Vertex")

        self.label_x = tk.Label(self.dialog, text="X:")
        self.entry_x = tk.Entry(self.dialog)
        self.label_x.pack()
        self.entry_x.pack()

        self.label_y = tk.Label(self.dialog, text="Y:")
        self.entry_y = tk.Entry(self.dialog)
        self.label_y.pack()
        self.entry_y.pack()

        self.label_z = tk.Label(self.dialog, text="Z:")
        self.entry_z = tk.Entry(self.dialog)
        self.label_z.pack()
        self.entry_z.pack()

        self.button_ok = tk.Button(self.dialog, text="OK", command=self.ok)
        self.button_cancel = tk.Button(self.dialog, text="Cancel", command=self.cancel)
        self.button_ok.pack()
        self.button_cancel.pack()

        self.vertex_id = vertex_id
        
        self.postdialog_callback = callback

        self.entry_x.insert(0, str(x))
        self.entry_y.insert(0, str(y))
        self.entry_z.insert(0, str(z))

    def ok(self):
        try:
            x = float(self.entry_x.get())
            y = float(self.entry_y.get())
            z = float(self.entry_z.get())
            self.x = x
            self.y = y
            self.z = z
            self.dialog.destroy()
            self.postdialog_callback(self.vertex_id, x, y, z)
        except ValueError:
            messagebox.showinfo("Error", "Invalid input.")

    def cancel(self):
        self.dialog.destroy()

if __name__ == "__main__":
    MapEditorGUI()