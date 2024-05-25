import pygame
from ui.uielement import *

class Button(UIElement):
    def __init__(self, 
                 text, 
                 action, 
                 x, y, 
                 width, height, 
                 parent=None,
                 color: tuple = (255, 255, 255),
                 pressed_color: tuple = (200, 200, 200),
                 outline_color: tuple = (0, 0, 0)
    ) -> None:
        super().__init__(x, y, width, height, parent, outlineColor=outline_color, elementColor=color)

        # Set the button text
        self.Text = text

        self.PressedColor = pressed_color
        self.DefaultColor = color

        # Set the action handler for this button
        self.Action = action

    # Draw the button to the screen
    def Draw(self):
        super().Draw(self.Text)

    # If the button was pressed, perform the action binded to the button
    def CheckPressed(self, mouseX, mouseY):
        if super().CheckPressed(mouseX, mouseY):
            self.Action()
            self.ElementColor = self.PressedColor
            return True
        self.ElementColor = self.DefaultColor
        return False
