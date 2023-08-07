import tkinter as tk
import math
from tkinter import messagebox
from tkinter import filedialog

class VertexDialog:
    def __init__(self, parent, x, y, z, callback):
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
            self.postdialog_callback(x, y, z)
        except ValueError:
            messagebox.showinfo("Error", "Invalid input.")


    def cancel(self):
        self.dialog.destroy()