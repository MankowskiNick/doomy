import pygame, sys
from pygame.locals import *
from map_builder import *
from ui.button import *
from ui.textbox import *
from map_file_handler import *

# TODO:
#   - Add editing

class Editor():
    def __init__(self, SCREENWIDTH, SCREENHEIGHT, title):
        pygame.init()

        self.ScreenWidth = SCREENWIDTH
        self.ScreenHeight = SCREENHEIGHT
        self.Screen = pygame.display.set_mode((SCREENWIDTH, SCREENHEIGHT))
        pygame.display.set_caption(title, icontitle=title)

        self.Running = True

        self.Canvas = Canvas(Map(), 0, 0, SCREENWIDTH, SCREENHEIGHT - 100)
        self.Buttons = [
            Button("Add wall", self.Canvas.ToggleEditMode,      (SCREENWIDTH * 1 // 8),     SCREENHEIGHT - 50,  80, 50),
            Button("Drag", self._ToggleDragMode,                (SCREENWIDTH * 2 // 8),     SCREENHEIGHT - 50,  80, 50),
            Button("New Map", self._CreateNewMap,               (SCREENWIDTH * 3 // 8),     SCREENHEIGHT - 50,  80, 50),
            Button("Process Sectors", self._ProcessSectors,     (SCREENWIDTH * 4 // 8),     SCREENHEIGHT - 50,  80, 50),
            Button("Clear Sectors", self._ClearSectors,         (SCREENWIDTH * 5 // 8),     SCREENHEIGHT - 50,  80, 50),
            Button("Save Map", self.SaveMap,                    (SCREENWIDTH * 6 // 8),     SCREENHEIGHT - 50,  80, 50),
            Button("Load Map", self.LoadMap,                    (SCREENWIDTH * 7 // 8),     SCREENHEIGHT - 50,  80, 50),
        ]

    def _ToggleDragMode(self):
        self.Canvas.ToggleDragMode()
        self.Buttons[1].Text = "Stop Dragging" if self.Canvas.DraggingMode else "Drag"

    def _CreateNewMap(self):
        self.Canvas.Map = Map()

    def _ProcessSectors(self): # TODO: Fix bug with this button(can't press it twice)
        # Process the bsp for the map and reassign self.map to be this
        bsp = BSP_Tree(self.Canvas.Map.GetFilteredMap())
        self.Canvas.Map = bsp.get_map()
        self.Canvas.BSP = bsp

    def _ClearSectors(self):
        self.Canvas.BSP = None

    def SaveMap(self):
        fileHandler = MapFileHandler(self.Canvas.Width, self.Canvas.Height, 0.05)
        fileHandler.SaveMap(self.Canvas.BSP, "lvldata.dat")
        return
    
    def LoadMap(self):
        self.Canvas.Map = None
        fileHandler = MapFileHandler(self.Canvas.Width, self.Canvas.Height, 0.05)
        self.Canvas.Map = fileHandler.LoadMap("lvldata.dat")

    def ShouldRun(self):
        return self.Running

    def HandleEvents(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.Running = False

            if event.type == pygame.MOUSEBUTTONDOWN:
                mouseX, mouseY = event.pos
                if self.Canvas.SurfaceRect.collidepoint(event.pos): # Note -- doesn't care about right/left click
                    self.Canvas.HandleClick(self.Screen, mouseX, mouseY)

                for button in self.Buttons:
                    if button.SurfaceRect.collidepoint(event.pos):
                        button.Action()

            if event.type == pygame.MOUSEBUTTONUP:
                self.Canvas.Dragging = False

            if event.type == pygame.MOUSEMOTION:
                mouseX, mouseY = event.pos
                self.Canvas.HandleDrag(mouseX, mouseY)


    def Draw(self):
        self.Screen.fill((127, 127, 127))
        # Update the canvas
        self.Canvas.Render()
        self.Screen.blit(self.Canvas.Surface, self.Canvas.SurfaceRect)
        for button in self.Buttons:
            button.Draw(self.Screen)

        pygame.display.update()

    def Exit(self):
        pygame.quit()
        sys.exit()

if __name__ == '__main__':
    editor = Editor(640, 480, "doomy Editor")

    while editor.ShouldRun():
        editor.HandleEvents()
        editor.Draw()
    editor.Exit()    
                    