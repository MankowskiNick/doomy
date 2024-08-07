import pygame
from pygame.locals import *
from typing import List, Any

BORDER_RADIUS = 0

class UIElement:
    def __init__(self, 
                    x: int, y: int, 
                    width: int, height: int, 
                    parent = None, 
                    outlineColor: tuple = (0, 0, 0), 
                    elementColor: tuple =(255, 255, 255), 
                    text: str = None,
                    text_color: tuple = (0, 0, 0),
                    fontsize: float = 10,
        ) -> None:
        self.x, self.y = x, y
        self._Width, self._Height = width, height
        self.Surface = pygame.Surface((width, height), pygame.SRCALPHA)
        self.SurfaceRect = self.Surface.get_rect(topleft=((x - (width // 2), y - (height // 2))))
        self.Parent = parent
        self.OutlineColor = outlineColor
        self.ElementColor = elementColor
        self.Text = text
        self.TextColor = text_color
        self.FontSize = fontsize

    def Draw(self, dispText: str = ""):
        # Draw the rectangular UI element
        pygame.draw.rect(self.Surface, self.OutlineColor, (0, 0, self._Width, self._Height), border_radius=BORDER_RADIUS)
        pygame.draw.rect(self.Surface, self.ElementColor, (1, 1, self._Width-2, self._Height-2), border_radius=BORDER_RADIUS)

        # Render text
        font = pygame.font.SysFont('Arial', self.FontSize)
        text = font.render(dispText if self.Text is None else self.Text, True, self.TextColor)
        text_rect = text.get_rect(center=(self._Width // 2, self._Height // 2))

        # Blit text to the surface
        self.Surface.blit(text, text_rect)

    # Blit to the screen.  This is separated from draw to avoid unintentional overdraw for subclasses.
    def Blit(self, screen):
        screen.blit(self.Surface, self.SurfaceRect)

    def CheckPressed(self, mouseX, mouseY):
        # Convert mouse coordinates to local coordinates
        cur_parent = self.Parent
        while cur_parent is not None:
            mouseX -= cur_parent.x - (cur_parent._Width // 2)
            mouseY -= cur_parent.y - (cur_parent._Height // 2)
            cur_parent = cur_parent.Parent
        return mouseX > self.x - (self._Width // 2) and mouseX < self.x + (self._Width // 2) and mouseY > self.y - (self._Height // 2) and mouseY < self.y + (self._Height // 2)
    
    def HandleDrag(self, mouseX, mouseY):
        pass