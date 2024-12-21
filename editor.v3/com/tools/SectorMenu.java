package com.tools;
import javax.swing.JComponent;

import com.actions.ProcessSectors;
import com.extensions.ActionButton;
import com.extensions.OptionMenu;
import com.canvas.Canvas;

public class SectorMenu extends OptionMenu
{
    SectorMenu(Canvas canvas)
    {
        super("Sector Options");

        this.Components = new JComponent[] 
        {
            new ActionButton("Process Sectors", new ProcessSectors(canvas)),
            new ActionButton("Edit Sector"),
            new ActionButton("Clear Sectors")
        };
        this.AttachComponents();
    }
}