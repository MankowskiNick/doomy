package com.tools;
import javax.swing.JComponent;

import com.extensions.ActionButton;
import com.extensions.OptionMenu;

public class WallMenu extends OptionMenu 
{
    WallMenu()
    {
        super("Wall Options");

        this.Components = new JComponent[] 
        {
            new ActionButton("Add Wall"),
            new ActionButton("Edit Wall"),
        };
        this.AttachComponents();
    }
}