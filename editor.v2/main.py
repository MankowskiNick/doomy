import pygame, sys
from pygame.locals import *
# from map_builder import *
from ui.button import *
from ui.textbox import *
from ui.toggleable import *
from ui.tray import *
from ui.popup import *
from canvas import Canvas
from map_file_handler import *

class Editor():
    def __init__(self, SCREENWIDTH, SCREENHEIGHT, title):
        
        # Initialize pygame and create window
        pygame.init()
        self.ScreenWidth = SCREENWIDTH
        self.ScreenHeight = SCREENHEIGHT
        self.Screen = pygame.display.set_mode((SCREENWIDTH, SCREENHEIGHT))
        self.Title = title
        pygame.display.set_caption(title)

        # Mark that the editor is running
        self.Running = True

        self.SidebarWidth = 250

        # Configure elements
        self.Elements = self.ConfigureElements()

        self.Canvas = self.Elements[0]

        self.MouseDown = False

        # Define file handler
        # self.FileHandler = MapFileHandler(self.Canvas.Width, self.Canvas.Height, 0.05)

    def ShouldRun(self):
        return self.Running

    def HandleEvents(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.Running = False

            if event.type == pygame.MOUSEBUTTONDOWN:
                mouseX, mouseY = event.pos
                self.MouseDown = True

                for element in self.Elements:
                    element.CheckPressed(mouseX, mouseY)

            if event.type == pygame.MOUSEBUTTONUP:
                self.MouseDown = False

            if event.type == pygame.MOUSEMOTION:
                mouseX, mouseY = event.pos
                if self.MouseDown:
                    for element in self.Elements:
                        element.HandleDrag(mouseX, mouseY)

    def Draw(self):
        self.Screen.fill((127, 127, 127))
        # Update the canvas
        # self.Canvas.Render()
        # self.Screen.blit(self.Canvas.Surface, self.Canvas.SurfaceRect)

        # Draw elements
        for element in self.Elements:
            element.Draw()
            element.Blit(self.Screen)

        pygame.display.update()

    def Run(self):
        while self.ShouldRun():
            self.HandleEvents()
            self.Draw()
        self.Exit()

    def Exit(self):
        pygame.quit()
        sys.exit()

    # EDITOR ACTIONS
            
    def ExportToBsp(self): # TODO: Fix bug with this button(can't press it twice)
        print("exporting to bsp...")
        print("TODO: get filename from user and export map to bsp")
        popup = Popup(
            x = self.ScreenWidth // 2, 
            y = self.ScreenHeight // 2,
            width = 2 * self.ScreenWidth // 3, 
            height = self.ScreenHeight // 2,
            screen = self.Screen,
            elements = [],
            elementColor = (255, 255, 255)
        )

        popup.Elements += [
            UIElement(
                text = 'Export Map',
                x = popup._Width // 2,
                y = popup._Height // 6,
                width = 2 * popup._Width // 3,
                height = popup._Height // 6,
                parent = popup,
                fontsize = 75,
                outlineColor = (255, 255, 255),
                elementColor = (255, 255, 255) 
            ),
            TextBox(
                x = popup._Width // 2,
                y = popup._Height // 2,
                width = 2 * popup._Width // 3,
                height = popup._Height // 6,
                defaultValue = 'map.bsp',
                parent = popup,
                fontsize = 25
            ),
            Button(
                text = 'Export',
                action = None, # TODO: write export functionality
                x = 1 * popup._Width // 3,
                y = 5 * popup._Height // 6,
                width = popup._Width // 4,
                height = popup._Height // 6,
                parent = popup,
                fontsize = 25
            ),
            Button(
                text = 'Cancel',
                action = popup.Destroy,
                x = 2 * popup._Width // 3,
                y = 5 * popup._Height // 6,
                width = popup._Width // 4,
                height = popup._Height // 6,
                parent = popup,
                fontsize = 25
            )
        ]
            
        popup.Popup()
            # Process the bsp for the map and reassign self.map to be this
            # bsp = BSP_Tree(self.Canvas.Map)
            # self.Canvas.Map = bsp.get_map()
            # self.Canvas.BSP = bsp
            # self.FileHandler.SaveMapBsp(bsp, "lvldata.bsp")

    def ProcessSectors(self):
        print("processing sectors...")
        print("TODO: process sectors")

    def EditSector(self):
        print("editing sector...")
        print("TODO: edit sector")
        
    def ClearSectors(self):
        print("clearing sectors...")
        print("TODO: clear sectors")
    #     self.Canvas.BSP = None

    def SetAddMode(self):
        print("setting add mode...")
        print("TODO: add mode")
        self.Canvas.EditMode = False
    
    def SetEditMode(self):
        print("setting edit mode...")
        print("TODO: edit mode")
        self.Canvas.EditMode = True

    def AddWall(self):
        print("adding wall...")
        print("TODO: add wall to map")
        self.Canvas.AddWallMode()


    def EditWall(self):
        print("editing wall...")
        print("TODO: edit wall on map")

    def CreateNewMap(self):
        print("creating new map...")
        print("TODO: cleanup?")
        # self.Canvas.Map = Map()

    def SaveMap(self):
        print("saving map...")
        print("TODO: actually save file")
        popup = Popup(
            x = self.ScreenWidth // 2, 
            y = self.ScreenHeight // 2,
            width = 2 * self.ScreenWidth // 3, 
            height = self.ScreenHeight // 2,
            screen = self.Screen,
            elements = [],
            elementColor = (255, 255, 255)
        )

        popup.Elements += [
            UIElement(
                text = 'Save File',
                x = popup._Width // 2,
                y = popup._Height // 6,
                width = popup._Width // 2,
                height = popup._Height // 6,
                parent = popup,
                fontsize = 75,
                outlineColor = (255, 255, 255),
                elementColor = (255, 255, 255) 
            ),
            TextBox(
                x = popup._Width // 2,
                y = popup._Height // 2,
                width = 2 * popup._Width // 3,
                height = popup._Height // 6,
                defaultValue = 'map.dat',
                parent = popup,
                fontsize = 25
            ),
            Button(
                text = 'Save',
                action = None, # TODO: write save functionality
                x = 1 * popup._Width // 3,
                y = 5 * popup._Height // 6,
                width = popup._Width // 4,
                height = popup._Height // 6,
                parent = popup,
                fontsize = 25
            ),
            Button(
                text = 'Cancel',
                action = popup.Destroy,
                x = 2 * popup._Width // 3,
                y = 5 * popup._Height // 6,
                width = popup._Width // 4,
                height = popup._Height // 6,
                parent = popup,
                fontsize = 25
            )
        ]
            
        popup.Popup()
        # self.FileHandler.SaveMapRaw(self.Canvas.Map, "lvldata.dat")
        return
    
    def LoadMap(self):
        print("loading map...")
        print("TODO: load appropriate map, check file exists")

        popup = Popup(
            x = self.ScreenWidth // 2, 
            y = self.ScreenHeight // 2,
            width = 2 * self.ScreenWidth // 3, 
            height = self.ScreenHeight // 2,
            screen = self.Screen,
            elements = [],
            elementColor = (255, 255, 255)
        )

        popup.Elements += [
            UIElement(
                text = 'Load File',
                x = popup._Width // 2,
                y = popup._Height // 6,
                width = popup._Width // 2,
                height = popup._Height // 6,
                parent = popup,
                fontsize = 75,
                outlineColor = (255, 255, 255),
                elementColor = (255, 255, 255) 
            ),
            TextBox(
                x = popup._Width // 2,
                y = popup._Height // 2,
                width = 2 * popup._Width // 3,
                height = popup._Height // 6,
                defaultValue = 'map.dat',
                parent = popup,
                fontsize = 25
            ),
            Button(
                text = 'Load',
                action = None, # TODO: write load functionality
                x = 1 * popup._Width // 3,
                y = 5 * popup._Height // 6,
                width = popup._Width // 4,
                height = popup._Height // 6,
                parent = popup,
                fontsize = 25
            ),
            Button(
                text = 'Cancel',
                action = popup.Destroy,
                x = 2 * popup._Width // 3,
                y = 5 * popup._Height // 6,
                width = popup._Width // 4,
                height = popup._Height // 6,
                parent = popup,
                fontsize = 25
            )
        ]
            
        popup.Popup()
        # self.Canvas.Map = Map() # TODO: Fix bug where maps do not properly clear before reloading
        # self.Canvas.Map = self.FileHandler.LoadMap("lvldata.dat")

    # UI CONFIGURATION
    def ConfigureElements(self) -> List[UIElement]:
        # Canvas
        canvas = self.ConfigureCanvas()

        # Overarching tray controlling the tools
        tools_tray = Tray(
                        title = "Tools",
                        x = self.ScreenWidth - (self.SidebarWidth // 2), y = self.ScreenHeight // 2,
                        width = self.SidebarWidth, height = self.ScreenHeight,
                        parent = None,
                        elements = [],
                        background_color = (255, 255, 255),
                        outline_color = (0, 0, 0))

        editor_options = self.ConfigureEditorOptions(tools_tray)
        wall_options = self.ConfigureWallOptions(tools_tray)
        sector_options = self.ConfigureSectorOptions(tools_tray)

        # Add editor options to the tools tray
        tools_tray.Elements.append(editor_options)
        tools_tray.Elements.append(wall_options)
        tools_tray.Elements.append(sector_options)

        return [canvas, tools_tray]

    # CANVAS CONFIGURATION
    def ConfigureCanvas(self) -> List[UIElement]:
        x, y = (self.ScreenWidth - self.SidebarWidth) // 2, self.ScreenHeight // 2
        canvas = Canvas(x, y, self.ScreenWidth - 250, self.ScreenHeight, self.Screen)
        return canvas

    # EDITOR OPTIONS
    # Editor options tray
    # 1. Mode - Edit/Add
    # 2. New map
    # 3. Save map
    # 4. Load map
    # 5. Export map to BSP
    def ConfigureEditorOptions(self, tools_tray: Tray) -> Tray:
        editor_options = Tray(
                            title = "Editor Options",
                            x = (tools_tray._Width // 2) + 5, y = 100,
                            width = tools_tray._Width - 15, height = 150,
                            parent = tools_tray,
                            elements = [],
                            background_color = (255, 255, 255),
                            outline_color = (0, 0, 0))
            
        # Mode toggle: Edit/Add
        mode_toggle = Toggleable(
                        text = 'Mode',
                        options = [],
                        x = (editor_options._Width // 2) + 5, y = 25,
                        width = tools_tray._Width - 10, height = 20,
                        parent = editor_options)
        mode_toggle.Elements.append(
            Button(
                text = "Edit", 
                action = self.SetEditMode,  
                x = mode_toggle._Width // 2, y = 10, 
                width = 50, height = 15,
                parent = mode_toggle)
        )
        mode_toggle.Elements.append(
            Button(
                text = "Add", 
                action = self.SetAddMode,  
                x = mode_toggle._Width * 5 // 6, y = 10, 
                width = 50, height = 15,
                parent = mode_toggle)
        )

        # New map button
        new_map = Button(
                    text = "New Map",
                    action = self.CreateNewMap,
                    x = (editor_options._Width // 2) + 5, y = 50,
                    width = editor_options._Width - 15, height = 20,
                    parent = editor_options)
        
        # Save map button
        save_map = Button(
                    text = "Save Map",
                    action = self.SaveMap,
                    x = (editor_options._Width // 2) + 5, y = 75,
                    width = editor_options._Width - 15, height = 20,
                    parent = editor_options)
        
        # Load map button
        load_map = Button(
                    text = "Load Map",
                    action = self.LoadMap,
                    x = (editor_options._Width // 2) + 5, y = 100,
                    width = editor_options._Width - 15, height = 20,
                    parent = editor_options)
        
        # Export map button
        export_map = Button(
                    text = "Export Map",
                    action = self.ExportToBsp,
                    x = (editor_options._Width // 2) + 5, y = 125,
                    width = editor_options._Width - 15, height = 20,
                    parent = editor_options)
        
        # Add editor option elements
        editor_options.Elements.append(mode_toggle)
        editor_options.Elements.append(new_map)
        editor_options.Elements.append(save_map)
        editor_options.Elements.append(load_map)
        editor_options.Elements.append(export_map)

        return editor_options

    # WALL OPTIONS
    # Wall options tray
    # 1. Add Wall
    # 2. Edit Wall
    def ConfigureWallOptions(self, tools_tray: Tray) -> Tray:
        wall_options = Tray(
                            title = "Wall Options",
                            x = (tools_tray._Width // 2) + 5, y = 255,
                            width = tools_tray._Width - 15, height = 150,
                            parent = tools_tray,
                            elements = [],
                            background_color = (255, 255, 255),
                            outline_color = (0, 0, 0))
        
        # Add wall button
        add_wall = Button(
                    text = "Add Wall",
                    action = self.AddWall,
                    x = (wall_options._Width // 2) + 5, y = 25,
                    width = wall_options._Width - 15, height = 20,
                    parent = wall_options)
        
        # Edit wall button
        edit_wall = Button(
                    text = "Edit Wall",
                    action = self.EditWall,
                    x = (wall_options._Width // 2) + 5, y = 50,
                    width = wall_options._Width - 15, height = 20,
                    parent = wall_options)

        # Add buttons to wall options tray
        wall_options.Elements.append(add_wall)
        wall_options.Elements.append(edit_wall)

        return wall_options
    
    # SECTOR OPTIONS
    # Sector options tray
    # 1. Process Sectors
    # 2. Edit sector
    # 3. Clear Sectors
    def ConfigureSectorOptions(self, tools_tray: Tray) -> Tray:
        sector_options = Tray(
                            title = "Sector Options",
                            x = (tools_tray._Width // 2) + 5, y = 410,
                            width = tools_tray._Width - 15, height = 150,
                            parent = tools_tray,
                            elements = [],
                            background_color = (255, 255, 255),
                            outline_color = (0, 0, 0))
        
        # Process sectors button
        process_sectors = Button(
                    text = "Process Sectors",
                    action = self.EditSector,
                    x = (sector_options._Width // 2) + 5, y = 25,
                    width = sector_options._Width - 15, height = 20,
                    parent = sector_options)
        
        # Edit sector button
        edit_sector = Button(
                    text = "Edit Sector",
                    action = self.ProcessSectors,
                    x = (sector_options._Width // 2) + 5, y = 50,
                    width = sector_options._Width - 15, height = 20,
                    parent = sector_options)
        
        # Clear sectors button
        clear_sectors = Button(
                    text = "Clear Sectors",
                    action = self.ClearSectors,
                    x = (sector_options._Width // 2) + 5, y = 75,
                    width = sector_options._Width - 15, height = 20,
                    parent = sector_options)
        
        # Add buttons to sector options tray
        sector_options.Elements.append(process_sectors)
        sector_options.Elements.append(edit_sector)
        sector_options.Elements.append(clear_sectors)

        return sector_options

if __name__ == '__main__':
    editor = Editor(1000, 750, "doomy Editor")
    editor.Run()