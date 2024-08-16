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
        Canvas.Vertices = new ArrayList<Vertex>();
        Canvas.Walls = new ArrayList<Wall>();
        Canvas.SelectedVertices = new ArrayList<Vertex>();
        Canvas.SelectedWalls = new ArrayList<Wall>();
        Canvas.repaint();
    }
}