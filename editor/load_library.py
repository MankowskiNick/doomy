import ctypes
import sys

def LoadSharedGraphics():
    # Determine the file extension based on the operating system
    if sys.platform.startswith("win"):
        extension = ".dll"
    elif sys.platform.startswith("darwin"):
        extension = ".dylib"
    else:
        extension = ".so"

    # Load shared graphics library (using ctypes)
    shared_graphics = ctypes.cdll.LoadLibrary(f"editor/bin/libshared_graphics{extension}")
    shared_graphics.GetWidth.restype = ctypes.c_int
    shared_graphics.GetHeight.restype = ctypes.c_int
    return shared_graphics
