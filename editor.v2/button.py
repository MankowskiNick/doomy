import pygame
from uielement import *

class Button(UIElement):
    def __init__(self, text, action, x, y, width, height, parent=None):
        super().__init__(x, y, width, height, parent, outlineColor=(0, 0, 0), elementColor=(255, 255, 255))

        # Set the button text
        self.Text = text

        # Set the action handler for this button
        self.Action = action

    # Draw the button to the screen
    def Draw(self, screen):
        super().Draw(self.Text)
        super().Blit(screen)

    # If the button was pressed, perform the action binded to the button
    def CheckPressed(self, mouseX, mouseY):
        if super().CheckPressed(mouseX, mouseY):
            self.Action()
