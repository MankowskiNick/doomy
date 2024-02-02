import pygame
from pygame.locals import *
from uielement import *
from textbox import *
from button import *

class Popup(UIElement):
    def __init__(self, x, y, width, height, screen, buttons, textboxes):
        super().__init__(x, y, width, height, outlineColor=(0, 0, 0), elementColor=(200, 200, 200))

        self._TextBoxes = textboxes
        self._Buttons = buttons

        self.Screen = screen
        self.Active = False

        self.OutData = None

    def Popup(self):
        self.Active = True
        while self.Active:
            for event in pygame.event.get():
                if event.type == MOUSEBUTTONDOWN:
                    mouseX, mouseY = event.pos
                    for button in self._Buttons:
                        button.CheckPressed(mouseX - self.x + (self._Width // 2), mouseY - self.y + (self._Height // 2))
                    for textbox in self._TextBoxes:
                        textbox.CheckPressed(self.Surface, mouseX - self.x + (self._Width // 2), mouseY - self.y + (self._Height // 2))
            self.Draw()
            pygame.display.update()


    def Draw(self):
        super().Draw("")
        for textbox in self._TextBoxes:
            textbox.Draw(self.Surface)
        for button in self._Buttons:
            button.Draw(self.Surface)
        super().Blit(self.Screen)
        
    def _Save(self):
        self._Destroy()

    def _Destroy(self):
        self.Active = False