from ui.stackableelement import StackableUIElement
from ui.uielement import UIElement


class ColorDataContainer(StackableUIElement):
    def __init__(self, 
                 x: int, y: int, 
                 width: int, height: int, 
                 parent: UIElement = None) -> None:
        super().__init__(
            x, y,
            width, height,
            parent=parent,
            elements=[
                
            ],
            outlineColor=(0, 0, 0),
            elementColor=(255, 255, 255))

        self.ColorData = None