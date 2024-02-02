from popup import *

class WallData:
    def __init__(self, color, wallMin, wallMax, floorHeight, ceilingHeight):
        self.Color = color
        self.WallMin = wallMin
        self.WallMax = wallMax
        self.FloorHeight = floorHeight
        self.CeilingHeight = ceilingHeight

class WallPopup(Popup):
    def __init__(self, x, y, width, height, screen):
        textboxes = [
            # RGB Input
            TextBox("R", width * 1 // 4, height * 1 // 5, width // 8, height // 7, "0", self),
            TextBox("G", width * 2 // 4, height * 1 // 5, width // 8, height // 7, "0", self),
            TextBox("B", width * 3 // 4, height * 1 // 5, width // 8, height // 7, "0", self),

            # Wall Height params
            TextBox("Wall Min Z", width * 1 // 3, height * 2 // 5, width // 6, height // 7, "0", self),
            TextBox("Wall Max Z", width * 2 // 3, height * 2 // 5, width // 6, height // 7, "0", self),

            TextBox("Floor Z", width * 1 // 3, height * 3 // 5, width // 6, height // 7, "-1", self),
            TextBox("Ceiling Z", width * 2 // 3, height * 3 // 5, width // 6, height // 7, "-1", self),
        ]

        buttons = [
            Button("Ok", self._Save, width * 1 // 3, height * 4 // 5, width // 4, height // 7, self),
            Button("Cancel", self._Destroy, width * 2 // 3, height * 4 // 5, width // 4, height // 7, self)
        ]

        super().__init__(x, y, width, height, screen, buttons, textboxes)
        
    def _Save(self):
        self.OutData = WallData([int(self._TextBoxes[0].Text) % 256, int(self._TextBoxes[1].Text) % 256, int(self._TextBoxes[2].Text) % 256], # Wall color
                                # Wall height params
                                float(self._TextBoxes[3].Text),
                                float(self._TextBoxes[4].Text),

                                # Floor/ceiling params
                                float(self._TextBoxes[5].Text),
                                float(self._TextBoxes[6].Text))
        self._Destroy()