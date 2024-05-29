import pygame
from pygame.locals import *
from ui.uielement import UIElement
from ui.stackableelement import StackableUIElement
from ui.textbox import TextBox
from ui.button import Button
from typing import List

class Popup(StackableUIElement):
    def __init__(self, 
                x, y, 
                width, height, 
                screen: pygame.Surface, 
                parent: UIElement = None,
                elements: List[UIElement] = []
    ) -> None:
        super().__init__(
            x, y, 
            width, height, 
            parent=parent,
            elements=elements,
            outlineColor=(0, 0, 0), 
            elementColor=(200, 200, 200))

        self.Screen = screen
        self.Active = False

    def Popup(self):
        self.Active = True
        while self.Active:
            for event in pygame.event.get():
                if event.type == MOUSEBUTTONDOWN:
                    mouseX, mouseY = event.pos
                    for element in self.Elements:
                        element.CheckPressed(mouseX, mouseY)
                if event.type == KEYDOWN:
                    if event.key == K_ESCAPE:
                        self.Destroy()

            self.Draw()
            super().Blit(self.Screen)
            pygame.display.update()


    def Draw(self):
        if self.Active:
            super().Draw()
            for element in self.Elements:
                element.Draw()
                self.Surface.blit(element.Surface, element.SurfaceRect)
        
    def Save(self):
        self._Destroy()

    def Destroy(self):
        self.Active = False