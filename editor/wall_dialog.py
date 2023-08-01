import tkinter as tk
import math
from tkinter import messagebox
from tkinter import filedialog

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
