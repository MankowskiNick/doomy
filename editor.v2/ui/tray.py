from typing import List
from ui.button import Button
from ui.toggleable import Toggleable
from ui.uielement import UIElement
from ui.stackableelement import StackableUIElement

class Tray(StackableUIElement):
    def __init__(self, 
                    title: str,
                    x: int, y: int, 
                    width: int, height: int,
                    parent: UIElement = None,
                    elements: List[UIElement] = [],
                    background_color: tuple = (255, 255, 255),
                    outline_color: tuple = (0, 0, 0)
    ) -> None:
        
       # Add label to the tray 
        elements = [
            UIElement(
                width // 2, 10,
                width, 15,
                self,
                background_color,
                background_color,
                title,
                (0, 0, 0)
            )
        ] + elements

        super().__init__(x, y, 
                        width, height, 
                        parent, 
                        elements,
                        outline_color, 
                        background_color)
        
        # Set the title of the tray
        self.Title = title