package com.tools;
import javax.swing.*;

import com.canvas.Canvas;
import com.extensions.OptionMenu;

public class Tools extends OptionMenu
{
    private Canvas Canvas;

    public Tools(Canvas canvas2)
    {
        this(200, 600, canvas2);
    }

    public Tools(int width, int height, Canvas canvas)
    {
        super("Tools");
        setSize(width, height);

        this.Canvas = canvas;

        this.Components = new JComponent[]
        {
            new FileMenu(this.Canvas),
            new WallMenu(),
            new SectorMenu()
        };
        this.AttachComponents();
        setFocusable(false);
    }
}