from ui.stackableelement import StackableUIElement
from ui.uielement import UIElement
from ui.popup import Popup
from map import Map

import pygame

from editor_math import *

LOOKDIST = 0.1

class Canvas(StackableUIElement):
    def __init__(self, 
                 x: int, y: int, 
                 width: int, height: int, 
                 screen: pygame.Surface) -> None:
        super().__init__(
                x,y, 
                width, height,
                parent = None,
                elements = [],
                outlineColor = (0, 0, 0),
                elementColor = (255, 255, 255))
        
        self.Map = Map()
        self.MapWidth = 50
        self.MapHeight = 50

        self.BSP = None

        self.EditMode = False

        self.Font = pygame.font.SysFont('Arial', 10)

        self.Screen = screen

        self.SelectedVertex = None
        self.SelectedWall = None

        self.AddingWall = False

    def Draw(self) -> None:
        super().Draw()
        # TODO: Draw vertices,  walls, and sectors
        for vert in self.Map.Vertices:
            drawX = int(vert.x * self.MapWidth) + self.x
            drawY = int(vert.y * self.MapHeight) + self.y
            pygame.draw.circle(self.Surface, (0, 0, 0), (drawX, drawY), 3)
            
            # If we have selected this vertex, draw a red circle around it
            if self.SelectedVertex is not None and self.SelectedVertex.id == vert.id:
                pygame.draw.circle(self.Surface, (255, 0, 0), (drawX, drawY), 5, 1)
            # Render vertex id
            id_text = self.Font.render("v_id:" + str(vert.id), True, (0,0,0))
            self.Surface.blit(id_text, (drawX + 5, drawY + 5))

    def CheckPressed(self, mouseX: int, mouseY: int) -> bool:
        element_pressed = super().CheckPressed(mouseX, mouseY)
        canvas_pressed = UIElement.CheckPressed(self, mouseX, mouseY)
        if not element_pressed and canvas_pressed:
            self.HandleClick(mouseX, mouseY)
            print(f'canvas pressed but no button: ({mouseX}, {mouseY})')
        return canvas_pressed
    
    def HandleDrag(self, mouseX, mouseY):
        super().HandleDrag(mouseX, mouseY)
        if self.SelectedVertex is not None and self.EditMode:
            self.SelectedVertex.x, self.SelectedVertex.y = self.ScreenToWorld(mouseX, mouseY)

    # Handle click event
    def HandleClick(self, mouseX: int, mouseY: int) -> None:

        # Convert screen coordinates to world coordinates
        worldX, worldY = self.ScreenToWorld(mouseX, mouseY)

        # Handle each mode
        if self.EditMode:
            self.HandleEditClick(worldX, worldY)
        else:
            self.HandleAddClick(worldX, worldY)

    # Map screen coordinate to world coordinate
    def ScreenToWorld(self, screenX: int, screenY: int) -> tuple[int, int]:
        worldX = (screenX - self.x) / self.MapWidth
        worldY = (screenY - self.y) / self.MapHeight

        return worldX, worldY
    
    # Handle click event in add mode
    def HandleAddClick(self, worldX: int, worldY: int) -> None:
        if self.SelectVertex(worldX, worldY):
            return
        self.Map.AddVertex(worldX, worldY)
        

    # Handle click event in edit mode
    def HandleEditClick(self, worldX: int, worldY: int) -> None:
        if self.SelectVertex(worldX, worldY):
            return
        self.SelectedVertex = None

    # Get vertex at world coordinates
    def GetVertex(self, x: float, y: float) -> Vertex:
        for vert in self.Map.Vertices:
            dist_to_vert = dist(x,y, vert.x, vert.y)
            if dist_to_vert < LOOKDIST:
                return vert
        return None

    # Attempt to select a vertex at world coordinates
    # Params: worldX, worldY - world coordinates
    # Returns: True if a vertex was selected/deselected, False otherwise
    def SelectVertex(self, worldX: float, worldY: float) -> bool:
        newVertex = self.GetVertex(worldX, worldY)


        if newVertex is None:
            return False
        
        if self.AddingWall:
            self.BuildWall(self.SelectedVertex, newVertex)
            self.AddingWall = False
            self.SelectedVertex = newVertex
            return True
        
        if self.SelectedVertex is not None and self.SelectedVertex.id == newVertex.id:
            self.SelectedVertex = None
            return True
        
        self.SelectedVertex = newVertex
        return True

    # Attempt to select a wall at world coordinates
    # Params: worldX, worldY - world coordinates
    # Returns: True if a wall was selected/deselected, False otherwise
    def SelectWall(self, worldX: float, worldY: float) -> bool:
        for wall in self.Map.Walls:
            sameWall = self.SelectedWall is not None and self.SelectedWall.id == wall.id
            if is_near_wall(worldX, worldY, wall) and not sameWall:
                self.SelectedWall = wall
                return True
            if sameWall:
                self.SelectedVertex = None
                return True
        return False
    
    def AddWallMode(self) -> None:
        if self.SelectedVertex is None:
            print("ERROR: No vertex selected")
            print("TODO: Make this a popup")
            return
        self.AddingWall = True

    def BuildWall(self, vert1: Vertex, vert2: Vertex) -> None:
        # Create a popup GUI to input wall information
        popup = Popup(
            x = self._Width // 2, y = self._Height // 2,
            width = self._Width * 2 // 3, height = self._Width * 2 // 3,
            screen = self.Screen,
            parent = self,
            elements = [

            ]
        )
        popup.Popup()
        # self.Elements.append(popup)