package com.actions;

import com.extensions.ActionWithDialog;
import com.canvas.Canvas;
import com.models.*;

import java.awt.event.ActionEvent;
import java.util.ArrayList;


public class NewMap extends ActionWithDialog
{
    private Canvas Canvas;

    public NewMap(Canvas canvas)
    {
        super();
        Canvas = canvas;
    }

    @Override
    public void actionPerformed(ActionEvent e) 
    {
        this.Canvas.Vertices = new ArrayList<Vertex>();
        this.Canvas.Walls = new ArrayList<Wall>();
        this.Canvas.SelectedVertices = new ArrayList<Vertex>();
        this.Canvas.SelectedWalls = new ArrayList<Wall>();
        this.Canvas.Tree = null;
        this.Canvas.repaint();
    }
}