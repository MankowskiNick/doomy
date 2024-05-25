import pygame
from pygame.locals import *
from ui.uielement import *
from ui.textbox import *
from ui.button import *

class Popup(UIElement):
    def __init__(self, x, y, width, height, screen, buttons, textboxes):
        super().__init__(x, y, width, height, outlineColor=(0, 0, 0), elementColor=(200, 200, 200))

        # self._TextBoxes = textboxes
        # self._Buttons = buttons

        self.Elements = textboxes + buttons

        self.Screen = screen
        self.Active = False

        self.OutData = None

    def Popup(self):
        self.Active = True
        while self.Active:
            for event in pygame.event.get():
                if event.type == MOUSEBUTTONDOWN:
                    mouseX, mouseY = event.pos
                    for element in self.Elements:
                        element.CheckPressed(mouseX, mouseY)

            self.Draw()
            super().Blit(self.Screen)
            pygame.display.update()


    def Draw(self):
        super().Draw()
        for element in self.Elements:
            # element.Draw(self.Surface)
            element.Draw()
            self.Surface.blit(element.Surface, element.SurfaceRect)
        # super().Blit(self.Screen)
        
    def _Save(self):
        self._Destroy()

    def _Destroy(self):
        self.Active = False