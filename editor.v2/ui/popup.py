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
                elements: List[UIElement] = [],
                outlineColor: tuple = (0, 0, 0),
                elementColor: tuple = (200, 200, 200)
    ) -> None:
        super().__init__(
            x, y, 
            width, height, 
            parent=parent,
            elements=elements,
            outlineColor=outlineColor, 
            elementColor=elementColor
        )

        self.Active = False
        self.Screen = screen

    def Popup(self):
        self.Active = True

        # Add x button
        self.Elements.append(
            Button(
                text = 'x', 
                action = self.Destroy, 
                x = 8,
                y = 8,
                width = 16, 
                height = 16,
                parent = self
            )
        )

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

    def Draw(self):
        if self.Active:
            super().Draw()
            self.Blit(self.Screen)
            pygame.display.update()

    def Blit(self, screen):
        if self.Active:
            super().Blit(screen)            
    
    def Save(self):
        self._Destroy()

    def Destroy(self):
        self.Active = False