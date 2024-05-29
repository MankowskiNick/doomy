from ui.stackableelement import StackableUIElement
from ui.uielement import UIElement
from map import Vertex


class VertexInfoContainer(StackableUIElement):
    def __init__(self, 
                 x, y, 
                 width, height, 
                 parent: UIElement, 
                 vertex: Vertex = None) -> None:
        super().__init__(
            x, y,
            width, height,
            parent=parent,
            elements=[
                UIElement(
                    x = width * 1 // 4, y = height // 2, 
                    width = (width // 2) - 10, height = height - 5, 
                    parent=self,
                    outlineColor=(0, 0, 0),
                    elementColor=(255, 255, 255),
                    text = f'Vertex #{vertex.id if vertex is not None else "None"}',
                    text_color = (0, 0, 0)),
                UIElement(
                    x = width * 3 // 4, y = height // 2, 
                    width = (width // 2) - 10, height = height - 5, 
                    parent=self,
                    outlineColor=(0, 0, 0),
                    elementColor=(255, 255, 255),
                    text = f'({vertex.x if vertex is not None else "None"}, {vertex.y if vertex is not None else "None"})',
                    text_color = (0, 0, 0)),

            ],
            outlineColor=(0, 0, 0),
            elementColor=(255, 255, 255))