from ui.uielement import UIElement
from typing import List

class StackableUIElement(UIElement):
    def __init__(self, 
                    x: int, y: int, 
                    width: int, height: int, 
                    parent = None, 
                    elements: List[UIElement] = [],
                    outlineColor: tuple = (0, 0, 0), 
                    elementColor: tuple =(255, 255, 255), 
                    text: str = None,
                    text_color: tuple = (0, 0, 0)
        ) -> None:
            super().__init__(x, y, width, height, parent, outlineColor, elementColor, text, text_color)
            self.Elements = elements
            for o in self.Elements:
                o.Parent = self
    
    def Draw(self):
        super().Draw()
        for e in self.Elements:
            e.Draw()
            self.Surface.blit(e.Surface, e.SurfaceRect)

    def CheckPressed(self, mouseX: int, mouseY: int) -> bool:
        pressed = False
        for element in self.Elements:
            if element.CheckPressed(mouseX, mouseY):
                pressed = True
        return pressed