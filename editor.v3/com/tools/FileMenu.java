package com.tools;
import java.util.ArrayList;

import javax.swing.*;

import com.actions.*;
import com.canvas.Canvas;
import com.canvas.EditMode;
import com.extensions.ActionButton;
import com.extensions.OptionMenu;
import com.extensions.Switch;
import com.models.Vertex;

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
                    canvas.SelectedVertices = new ArrayList<Vertex>();
                    System.out.println("Selected vertex cleared");
                },
                () -> {
                    canvas.Mode = EditMode.EDIT;
                    System.out.println("Mode set to EDIT");
                }
            ),
            new ActionButton("New Map", new NewMap(canvas)),
            new ActionButton("Save Map"),
            new ActionButton("Load Map"),
            new ActionButton("Export Map")
        };

        this.AttachComponents();
    }
}