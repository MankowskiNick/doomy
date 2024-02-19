import pygame
from pygame.locals import *

BORDER_RADIUS = 10

class UIElement:
    def __init__(self, x, y, width, height, parent=None, outlineColor=(0, 0, 0), elementColor=(255, 255, 255)):
        self.x, self.y = x, y
        self._Width, self._Height = width, height
        self.Surface = pygame.Surface((width, height), pygame.SRCALPHA)
        self.SurfaceRect = self.Surface.get_rect(topleft=((x - (width // 2), y - (height // 2))))
        self.Parent = parent
        self.OutlineColor = outlineColor
        self.ElementColor = elementColor

    def Draw(self, dispText):
        # Draw the rectangular UI element
        pygame.draw.rect(self.Surface, self.OutlineColor, (0, 0, self._Width, self._Height), border_radius=BORDER_RADIUS)
        pygame.draw.rect(self.Surface, self.ElementColor, (1, 1, self._Width-2, self._Height-2), border_radius=BORDER_RADIUS)

        # Render text
        font = pygame.font.SysFont('Arial', 10)
        text = font.render(dispText, True, (0, 0, 0))
        text_rect = text.get_rect(center=(self._Width // 2, self._Height // 2))

        # Blit text to the surface
        self.Surface.blit(text, text_rect)

    # Blit to the screen.  This is separated from draw to avoid unintentional overdraw for subclasses.
    def Blit(self, screen):
        screen.blit(self.Surface, self.SurfaceRect)

    def CheckPressed(self, mouseX, mouseY): # TODO: use collidepoint to make this easier to read?
        return mouseX > self.x - (self._Width // 2) and mouseX < self.x + (self._Width // 2) and mouseY > self.y - (self._Height // 2) and mouseY < self.y + (self._Height // 2)