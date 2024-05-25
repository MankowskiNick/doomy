from typing import List
from ui.button import Button
from ui.toggleable import Toggleable
from ui.uielement import UIElement

class Tray(UIElement):
    def __init__(self, 
                    title: str,
                    x: int, y: int, 
                    width: int, height: int,
                    parent: UIElement = None,
                    elements: List[UIElement] = [],
                    background_color: tuple = (255, 255, 255),
                    outline_color: tuple = (0, 0, 0)
    ) -> None:
        super().__init__(x, y, 
                        width, height, 
                        parent, 
                        outline_color, 
                        background_color)
        
        # Set the title of the tray
        self.Title = title
        
        # Define UI buttons
        self.Elements = elements

        self.Elements.append(
            UIElement(
                self._Width // 2, 10,
                width, 15,
                self,
                background_color,
                background_color,
                self.Title,
                (0, 0, 0)
            )
        )

    def Draw(self) -> None:
        super().Draw()
        for element in self.Elements:
            element.Draw()
            self.Surface.blit(element.Surface, element.SurfaceRect)
    
    def CheckPressed(self, mouseX: int, mouseY: int) -> bool:
        pressed = False
        for element in self.Elements:
            if element.CheckPressed(mouseX, mouseY):
                pressed = True
        return pressed