package com.tools;
import javax.swing.*;

import com.canvas.Canvas;
import com.canvas.EditMode;
import com.extensions.ActionButton;
import com.extensions.OptionMenu;
import com.extensions.Switch;

public class FileMenu extends OptionMenu
{
    FileMenu(Canvas canvas)
    {
        super("File");

        this.Components = new JComponent[] 
        {
            new JLabel("Mode"),
            new Switch("Add", "Edit", 
                () -> 
                { 
                    canvas.Mode = EditMode.ADD; 
                    System.out.println("Mode set to ADD");
                    canvas.SelectedVertex = null;
                    System.out.println("Selected vertex cleared");
                },
                () -> {
                    canvas.Mode = EditMode.EDIT;
                    System.out.println("Mode set to EDIT");
                }
            ),
            new ActionButton("New Map"),
            new ActionButton("Save Map"),
            new ActionButton("Load Map"),
            new ActionButton("Export Map")
        };

        this.AttachComponents();
    }
}