package com.tools;
import javax.swing.JComponent;

import com.extensions.ActionButton;
import com.extensions.OptionMenu;

public class SectorMenu extends OptionMenu
{
    SectorMenu()
    {
        super("Sector Options");

        this.Components = new JComponent[] 
        {
            new ActionButton("Process Sectors"),
            new ActionButton("Edit Sector"),
            new ActionButton("Clear Sectors")
        };
        this.AttachComponents();
    }
}