package com.tools;
import javax.swing.JComponent;

import com.extensions.ActionButton;
import com.extensions.OptionMenu;
import com.actions.*;
import com.canvas.Canvas;

public class WallMenu extends OptionMenu 
{
    public WallMenu(Canvas canvas)
    {
        super("Wall Options");

        this.Components = new JComponent[] 
        {
            new ActionButton("Add Wall", new AddWall(canvas)),
            new ActionButton("Edit Wall", new EditWall(canvas)),
        };
        this.AttachComponents();
    }
}