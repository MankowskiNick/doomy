# class WallPopup(StackableUIElement):
#     def __init__(self, x, y, width, height, parent, screen) -> None:
#         super().__init__(
#                 x,y, 
#                 width, height,
#                 parent = parent,
#                 elements = [
#                     TextBox("Width:", x + 10, y + 10, 100, 30, defaultValue = "100"),
#                     TextBox("Height:", x + 10, y + 50, 100, 30, defaultValue = "100"),
#                     Button("Add Wall", x + 10, y + 90, 100, 30, self.AddWall)
#                 ],
#                 outlineColor = (0, 0, 0),
#                 elementColor = (255, 255, 255))
        
        
#     def AddWall(self) -> None:
#         self.WallWidth = int(self.WallWidthInput.Text)
#         self.WallHeight = int(self.WallHeightInput.Text)
        
#         self.Wall = Wall(0, 0, self.WallWidth, self.WallHeight)
#         self.Wall.SetVertices()
        
#         self.Screen.Map.AddWall(self.Wall)
        
#         self.Destroy()
        
#     def Draw(self) -> None:
#         super().Draw()
#         self.WallWidthInput.Draw()
#         self.WallHeightInput.Draw()
#         self.AddButton.Draw()
        
#     def CheckPressed(self, mouseX: int, mouseY: int) -> bool:
#         element_pressed = super().CheckPressed(mouseX, mouseY)
#         if not element_pressed:
#             self.Destroy()
#         return element_pressed
        
#     def Destroy(self) -> None:
#         self.Parent.RemoveElement(self)
#         self.Parent = None
#         self.WallWidthInput.Destroy()
#         self.WallHeightInput.Destroy()
#         self.AddButton.Destroy()
#         self.WallWidthInput = None
#         self.WallHeightInput = None
#         self.AddButton = None
#         self.Wall = None
#         self.Screen = None