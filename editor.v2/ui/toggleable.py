from typing import List, Callable

import pygame
from ui.uielement import UIElement
from ui.button import Button

class Toggleable(UIElement):
    def __init__(self, 
                 text: str, 
                 options: List[Button],
                 x: int, y: int,
                width: int, height: int,
                parent: UIElement = None,
                color: tuple = (255, 255, 255),
                active_color: tuple = (200, 200, 200),
    ) -> None:
        super().__init__(x, y, width, height, parent, color, color)
        self.Label = UIElement(width // 6, 10, width // 4, 15, self, color, color, text, (0, 0, 0))
        self.Options = options
        self.SelectedOption = None

        self.DefaultColor = color
        self.ActiveColor = active_color

        for o in self.Options:
            o.Parent = self
            o.ElementColor = self.DefaultColor

    def Draw(self) -> None:
        super().Draw()
        self.Label.Draw()
        self.Surface.blit(self.Label.Surface, self.Label.SurfaceRect)
        for option in self.Options:
            option.Draw()
            self.Surface.blit(option.Surface, option.SurfaceRect)

    def CheckPressed(self, mouseX: int, mouseY: int) -> bool:

        # Keep track of if an option was pressed, since CheckPressed() will 
        # reset button colors, we need to make sure all options are checked.
        pressed = False
        for option in self.Options:

            # If the option was pressed, set the selected option to the pressed option
            # and change its color to indicate it was selected.
            if option.CheckPressed(mouseX, mouseY):
                self.SelectedOption = option
                self.SelectedOption.ElementColor = self.ActiveColor
                pressed = True

        # Return pressed status
        return pressed