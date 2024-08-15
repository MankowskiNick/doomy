package com.canvas;
import javax.swing.*;

import com.models.Vertex;
import com.models.Wall;

import java.awt.*;
import java.util.ArrayList;
import java.lang.Math;

public class Canvas extends JPanel
{
    public ArrayList<Vertex> Vertices = new ArrayList<Vertex>();
    public ArrayList<Wall> Walls = new ArrayList<Wall>();

    public EditMode Mode = EditMode.ADD;

    public int NextVertexId = 0;
    public int NextWallId = 0;

    public ArrayList<Vertex> SelectedVertices = new ArrayList<Vertex>();
    public boolean MultiSelectMode = false;

    private final float SelectionDistThreshold = 6;
    private final int VertexDrawRadius = 3;

    public boolean DragMode = false;

    public Canvas()
    {
        this(600, 600);
    }

    public Canvas(int width, int height)
    {
        setSize(width, height);
        setBackground(Color.WHITE);    
        setFocusable(false);
    }

    @Override
    protected void paintComponent(Graphics g)
    {
        // call super
        super.paintComponent(g);

        // configure graphics
        Graphics2D g2d = (Graphics2D) g;
        g2d.setColor(Color.BLACK);

        // draw vertices
        for (Vertex v : this.Vertices)
        {
            g2d.fillOval(
                (int)(v.x - this.VertexDrawRadius), (int)(v.y - this.VertexDrawRadius), 
                2 * this.VertexDrawRadius, 2 * this.VertexDrawRadius
            );
        }

        // draw walls
        for (Wall w : this.Walls)
        {
            g2d.setColor(new Color(w.Color[0], w.Color[1], w.Color[2]));
            g2d.drawLine(
                (int)w.Line.a.x, (int)w.Line.a.y,
                (int)w.Line.b.x, (int)w.Line.b.y
            );
        }

        // draw outline around SelectedVertices
        for (Vertex v : this.SelectedVertices)
        {
            g2d.setColor(Color.RED);
            g2d.drawOval(
                (int)(v.x - this.VertexDrawRadius - 1), (int)(v.y - this.VertexDrawRadius - 1),
                2 * (this.VertexDrawRadius + 1), 2 * (this.VertexDrawRadius + 1)    
            );
        }
    }

    public void AddVertex(int x, int y)
    {
        this.Vertices.add(new Vertex(this.NextVertexId, x, y));
        this.NextVertexId++;
        repaint();
    }

    public void SelectVertex(int x, int y)
    {
        if (!this.MultiSelectMode)
            this.SelectedVertices = new ArrayList<Vertex>();
        for (Vertex v : this.Vertices)
        {
            double distTo = Math.pow(Math.pow(x - v.x, 2) + Math.pow(y - v.y, 2), 0.5);
            if (distTo < this.SelectionDistThreshold)
            {
                this.SelectedVertices.add(v);
                break;
            }
        }
        repaint();
    }
}