import tkinter as tk
import map
import math
from tkinter import messagebox

def rgb_to_hex(rgb):
    return '#%02x%02x%02x' % tuple(rgb)

class MapEditorGUI:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Map Editor")
        self.canvas_width = 600
        self.canvas_height = 400
        self.canvas = tk.Canvas(self.root, width=self.canvas_width, height=self.canvas_height, bg="white")
        self.canvas.pack()

        self.map = map.CreateEmptyMap()
        self.selected_vertex = None
        self.selected_wall = None

        self.canvas.bind("<Button-1>", self.on_canvas_click)

        self.is_vertex_mode = True
        self.mode_button = tk.Button(self.root, text = "Mode: " + "Add Vertices" if self.is_vertex_mode else "Add Walls", command=self.swap_vertex_mode)
        self.mode_button.pack()

        self.create_menu()
        self.root.mainloop()

    def create_menu(self):
        menubar = tk.Menu(self.root)
        file_menu = tk.Menu(menubar, tearoff=0)
        file_menu.add_command(label="New Map", command=self.new_map)
        file_menu.add_command(label="Save Map", command=self.save_map)
        file_menu.add_command(label="Load Map", command=self.load_map)
        menubar.add_cascade(label="File", menu=file_menu)

        edit_menu = tk.Menu(menubar, tearoff=0)
        edit_menu.add_command(label="Add Vertex", command=self.add_vertex)
        #edit_menu.add_command(label="Add Wall", command=self.add_wall)
        edit_menu.add_command(label="Delete Vertex", command=self.delete_vertex)
        edit_menu.add_command(label="Delete Wall", command=self.delete_wall)
        menubar.add_cascade(label="Edit", menu=edit_menu)

        self.root.config(menu=menubar)

    def on_canvas_click(self, event):
        x, y = event.x, event.y
        if (self.is_vertex_mode):
            if(self.select_vertex(event)):
                self.redraw_canvas()
                return
            self.create_vertex(x, y)
            self.redraw_canvas()
        else:
            if self.selected_vertex is None:
                self.select_vertex(event)
                self.redraw_canvas()
                #messagebox.showinfo("Error", "Please select a vertex first.")
                return
            
            cur_vertex = self.get_vertex(x, y)
            if (cur_vertex != None):
                self.create_wall(cur_vertex, self.selected_vertex)

    def create_vertex(self, x, y):
        vertex_id = len(self.map.Vertices) + 1
        vertex = map.Vertex(vertex_id, x, y, 0)
        self.map.AddVertex(vertex_id, x, y, 0)
        self.canvas.create_oval(x - 5, y - 5, x + 5, y + 5, fill="black")
        self.canvas.create_text(x + 10, y - 10, text=str(vertex_id))

    def create_wall(self, v1, v2):
        if self.selected_vertex.id:
            wall_height = 50
            wall_color = [255, 0, 0]  # Default color: red

            wall_dialog = WallDialog(self.root, v1, v2, self.build_wall)
        else:
            messagebox.showinfo("Error", "Please select a vertex first.")
    
    def build_wall(self, v1, v2, height, color):
        self.map.AddWall(v1, v2, height, *color)
        self.redraw_canvas()

    def select_vertex(self, event):
        x = event.x
        y = event.y

        # Iterate over the vertices and check if the click coordinates are within a certain range
        potential_vertex = self.get_vertex(x, y)
        if (potential_vertex != None and potential_vertex == self.selected_vertex):
            self.selected_vertex = None
        else:
            self.selected_vertex = self.get_vertex(x, y)
        return self.selected_vertex != None

    def get_vertex(self, x, y):
        for vertex in self.map.Vertices:
            if abs(vertex.x - x) <= 5 and abs(vertex.y - y) <= 5:
                # Return the vertex
                return vertex
        return None

    def swap_vertex_mode(self):
        self.is_vertex_mode = False if self.is_vertex_mode else True
        self.mode_button.configure(text="Mode: " + ("Add Vertices" if self.is_vertex_mode else "Add Walls"))

    def select_wall(self, event):
        x, y = event.x, event.y
        items = self.canvas.find_closest(x, y)
        if items:
            item = items[0]
            tags = self.canvas.gettags(item)
            if tags:
                wall_id = int(tags[0])
                self.selected_wall = self.get_wall_by_id(wall_id)

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

    def build_vertex(self, id, x, y, z):
        self.map.AddVertex(id, x, y, z)
        self.redraw_canvas()

    def add_vertex(self):
        vertex_id = len(self.map.Vertices) + 1
        vertex_dialog = VertexDialog(self.root, vertex_id, build_vertex)

    def redraw_canvas(self):
        self.canvas.delete("all")
        for vertex in self.map.Vertices:
            x, y = vertex.x, vertex.y
            if (self.selected_vertex != None and self.selected_vertex.id == vertex.id):
                self.canvas.create_oval(x - 7, y - 7, x + 7, y + 7, fill="red")
            self.canvas.create_oval(x - 5, y - 5, x + 5, y + 5, fill="black")
            self.canvas.create_text(x + 10, y - 10, text=str(vertex.id))

        for wall in self.map.Walls:
            x1, y1 = wall.line.v1.x, wall.line.v1.y
            x2, y2 = wall.line.v2.x, wall.line.v2.y
            color = rgb_to_hex(wall.color)
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
        self.map = CreateEmptyMap()
        self.canvas.delete("all")

    def save_map(self):
        # TODO: Save functionality
        pass

    def load_map(self):
        # TODO: Load functionality
        pass

class WallDialog:
    def __init__(self, parent, v1, v2, callback):
        self.result = None

        self.dialog = tk.Toplevel(parent)
        self.dialog.title("Add Wall")

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

    def ok(self):
        try:
            wall_height = int(self.entry_height.get())
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
    def __init__(self, parent, vertex_id, callback):
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

    def ok(self):
        try:
            x = float(self.entry_x.get())
            y = float(self.entry_y.get())
            z = float(self.entry_z.get())
            self.result = True
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
