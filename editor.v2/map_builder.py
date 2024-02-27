import pygame, easygui, sys, random
from editor_math import *
from ui.wallpopup import *

LOOKDIST = 15

def get_random_color():
    """Returns a random color as an (R, G, B) tuple."""
    red = random.randint(0, 255)
    green = random.randint(0, 255)
    blue = random.randint(0, 255)
    return (red, green, blue)


# Map Editor should handle inputs & provide buttons to render
class Canvas:
    def __init__(self, new_map, x, y, width, height):
        self.Map = Map()
        self.BSP = None

        self.Surface = pygame.Surface((width, height))
        self.SurfaceRect = self.Surface.get_rect(topleft=(x, y))

        self.Width, self.Height = width, height

        # VertexMode == True => clicking on canvas will add new vertex
        # VertexMode == False => clicking on canvas will add walls
        self.VertexMode = True

        self.DraggingMode = False

        # Are we currently dragging an item across the screen?
        self.Dragging = False

        self._SelectedVertex = None
        self._SelectedWall = None

        self._Buttons = []

    def ToggleEditMode(self):
        self.VertexMode = not self.VertexMode
    
    def ToggleDragMode(self):
        self.DraggingMode = not self.DraggingMode

    def SelectVertex(self, x, y):
        newVertex = self.GetVertex(x, y)
        if newVertex is None:
            return False
        if self._SelectedVertex is not None and self._SelectedVertex.id == newVertex.id:
            self._SelectedVertex = None
            return False
        self._SelectedVertex = newVertex 
        return True

    def GetVertex(self, x, y):
        for vert in self.Map.Vertices:
            withinDist = dist(x,y, vert.x, vert.y) <= LOOKDIST
            if withinDist:
                return vert
        return None

    def SelectWall(self, x, y):
        for wall in self.Map.Walls:
            sameWall = self._SelectedWall is not None and self._SelectedWall.id == wall.id
            if is_near_wall(x, y, wall) and not sameWall:
                self._SelectedWall = wall
                return True
            elif sameWall:
                self._SelectedVertex = None
        return False

    def HandleDrag(self, x, y):
        if self.Dragging:
            dragVertex = self.GetVertex(x, y)
            if dragVertex is not None:
                dragVertex.x = x
                dragVertex.y = y

    def HandleClick(self, screen, x, y):
        if self.DraggingMode:
            vert = self.GetVertex(x, y)
            if vert is not None:
                self._SelectedVertex = vert
                self.Dragging = True
            else:
                self._SelectedVertex = None
            return

        # # If we are adding vertices rather than walls,
        if (self.VertexMode):
            # If we can select a vertex, select one
            if self.SelectVertex(x, y):
                return

            # If we can select a wall, select one
            if self.SelectWall(x, y):
                return

            self.Map.AddVertex(x, y)

        # If we are adding walls rather than vertices,
        else:
            if self._SelectedVertex is None:
                if not self.SelectVertex(x,y):
                    print("ERROR: Must select a vertex to create a wall")
                return
            curVertex = self._SelectedVertex

            if not self.SelectVertex(x, y):
                self._SelectedVertex = self.Map.AddVertex(x, y)

            # Create a popup GUI to input wall information
            screenWidth, screenHeight = pygame.display.Info().current_w, pygame.display.Info().current_h
            wallPopup = WallPopup(screenWidth // 2, screenHeight // 2, 
                            screenWidth // 2, screenHeight // 2, 
                            screen)
            wallPopup.Popup()

            # If wall data was written to wallPopup.OutData, that means the wall was valid and can be used to create a new wall
            if wallPopup.OutData is not None:
                wallData = wallPopup.OutData
                self.Map.AddWall(curVertex.id, 
                                self._SelectedVertex.id, 
                                wallData.Color, 
                                wallData.WallMin, 
                                wallData.WallMax, 
                                wallData.FloorHeight, 
                                wallData.CeilingHeight)
            
            # Go back to vertex mode
            self.VertexMode = True

    def Render(self):
        # Fill the Canvas with white
        self.Surface.fill((255,255,255))
        
        # Draw sectors
        if self.BSP is not None:
            for sector in self.BSP.get_sectors():
                walls = [self.Map.GetWallById(w.id) for w in sector.walls]
                lines = [w.line for w in walls]
                vertices = []
                for line in lines:
                    v1, v2 = line.v1, line.v2
                    if v1 not in vertices:
                        vertices.append(v1)
                    if v2 not in vertices:
                        vertices.append(v2)
                if len(vertices) > 0:
                    vertices = sort_vertices(vertices)
                    points = [(v.x, v.y) for v in vertices]
                    if len(points) > 3:
                        pygame.draw.polygon(self.Surface, (150,150,150), points)
                        pygame.draw.polygon(self.Surface, (200,200,200), points, 5)
        
        # Draw vertices
        for vert in self.Map.Vertices:
            pygame.draw.circle(self.Surface, (0, 0, 0), (vert.x, vert.y), 3)
            # If we have selected this vertex, draw a red circle around it
            if self._SelectedVertex is not None and self._SelectedVertex.id == vert.id:
                pygame.draw.circle(self.Surface, (255, 0, 0), (vert.x, vert.y), 5, 1)
        
        # Draw walls
        # Initialize a font object
        myfont = pygame.font.SysFont('Arial', 14)
    
        for wall in self.Map.Walls:
            if self._SelectedWall is not None and self._SelectedWall.id == wall.id:
                pygame.draw.line(self.Surface, wall.color, (wall.line.v1.x, wall.line.v1.y), (wall.line.v2.x, wall.line.v2.y), width=3)
            else:
                pygame.draw.line(self.Surface, wall.color, (wall.line.v1.x, wall.line.v1.y), (wall.line.v2.x, wall.line.v2.y), width=1)
            # Calculate the midpoint of the wall to place the ID text
            mid_x = (wall.line.v1.x + wall.line.v2.x) / 2
            mid_y = (wall.line.v1.y + wall.line.v2.y) / 2

            # Render the wall ID text
            id_text = myfont.render(str(wall.id), True, (0, 0, 0))  # Black color text

            # Blit the text onto the surface at the midpoint of the wall
            self.Surface.blit(id_text, (mid_x, mid_y))