
import pygame
from pygame.locals import *
from ui.uielement import *

class TextBox(UIElement):
    def __init__(
        self, 
        x, y, 
        width, height, 
        text = "",
        defaultValue = "", 
        parent = None, 
        fontsize = 10
    ):        
        super().__init__(
            x, y, 
            width, height, 
            parent, 
            outlineColor=(0, 0, 0), 
            elementColor=(255, 255, 255), 
            fontsize=fontsize
        )

        # Set the default text and text parameters
        self._DefaultText = text
        self._DefaultValue = defaultValue
        self.Text = defaultValue

        # Set the screen
        self.Screen = parent.Screen if parent is not None else None

        self.Active = False
        
    # Draw textbox to surface
    def Draw(self):
        super().Draw(self._DefaultText if self.Text == "" else self.Text)

    # Check if the textbox was pressed, if so, make it active
    def CheckPressed(self, mouseX, mouseY):
        if super().CheckPressed(mouseX, mouseY):
            # Set the element color to a light blue to show focus on this UI element
            self.ElementColor = (200,200,255)
            # Handle typing
            self.HandleInput()
    
    def HandleInput(self):
        # Are we still inputting text?
        self.Active = True

        while (self.Active):
            for event in pygame.event.get():
                # Typing
                if event.type == KEYDOWN:
                    # If we are exiting the textbox
                    if event.key == pygame.K_RETURN or event.key == pygame.K_KP_ENTER:
                        self.Active = False
                    # If we are hitting backspace
                    elif event.key == pygame.K_BACKSPACE and self.Text != "":
                        self.Text = self.Text[:-1]
                    # Otherwise, we are typing a character
                    else:
                        if self.Text == self._DefaultValue:
                            self.Text = ""
                        self.Text += event.unicode

            # Draw the parent to update the screen
            self.Parent.Draw()

            # Update the surface -- needed?
            self.Draw()

            # Update the screen
            pygame.display.update()
        
        # Once we have exited, change the element color back to white
        self.ElementColor = (255, 255, 255)