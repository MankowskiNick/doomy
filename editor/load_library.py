import ctypes

def LoadSharedGraphics():    
    # Load shared graphics library(using ctypes)
    shared_graphics = ctypes.cdll.LoadLibrary("editor/bin/libshared_graphics.so")
    shared_graphics.GetWidth.restype = ctypes.c_int
    shared_graphics.GetHeight.restype = ctypes.c_int
    return shared_graphics