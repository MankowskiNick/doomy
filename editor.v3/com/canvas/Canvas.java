package com.canvas;
import javax.swing.*;

import com.models.*;

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
    public ArrayList<Wall> SelectedWalls = new ArrayList<Wall>();
    public boolean MultiSelectMode = false;

    private static final float SelectionDistThreshold = 6;
    private static final int VertexDrawRadius = 3;
    private static final int LineThickness = 1;
    private static final int SelectedLineThickness = 2;

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

        // draw walls
        for (Wall w : this.Walls)
        {
            g2d.setColor(new Color(w.Color[0], w.Color[1], w.Color[2]));
            g2d.setStroke(new BasicStroke(LineThickness));
            g2d.drawLine(
                (int)w.Line.a.x, (int)w.Line.a.y,
                (int)w.Line.b.x, (int)w.Line.b.y
            );
        }
        // draw selected walls
        for (Wall w : this.SelectedWalls)
        {            
            g2d.setColor(new Color(w.Color[0], w.Color[1], w.Color[2]));
            g2d.setStroke(new BasicStroke(SelectedLineThickness));
            g2d.drawLine(
                (int)w.Line.a.x, (int)w.Line.a.y,
                (int)w.Line.b.x, (int)w.Line.b.y
            );
        }
        
        // draw vertices
        for (Vertex v : this.Vertices)
        {
            g2d.setColor(Color.BLACK);
            g2d.fillOval(
                (int)(v.x - VertexDrawRadius), (int)(v.y - VertexDrawRadius), 
                2 * VertexDrawRadius, 2 * VertexDrawRadius
            );
        }

        // draw outline around SelectedVertices
        for (Vertex v : this.SelectedVertices)
        {
            g2d.setColor(Color.RED);
            g2d.setStroke(new BasicStroke(LineThickness));
            g2d.drawOval(
                (int)(v.x - VertexDrawRadius - 1), (int)(v.y - VertexDrawRadius - 1),
                2 * (VertexDrawRadius + 1), 2 * (VertexDrawRadius + 1)    
            );
        }
    }

    public void AddVertex(int x, int y)
    {
        this.Vertices.add(new Vertex(this.NextVertexId, x, y));
        this.NextVertexId++;
        repaint();
    }

    public boolean SelectVertex(int x, int y)
    {
        // clear selection if not in MultiSelectMode
        if (!this.MultiSelectMode)
            this.SelectedVertices = new ArrayList<Vertex>();

        // select vertex if within SelectionDistThreshold
        for (Vertex v : this.Vertices)
        {
            double distTo = Math.pow(Math.pow(x - v.x, 2) + Math.pow(y - v.y, 2), 0.5);
            if (distTo < SelectionDistThreshold)
            {
                // add vertex to SelectedVertices & return true
                this.SelectedVertices.add(v);
                repaint();
                return true;
            }
        }

        // return false if no vertex was selected
        repaint();
        return false;
    }

    public boolean SelectWall(int x, int y)
    {
        // clear selection if not in MultiSelectMode
        if (!this.MultiSelectMode)
            this.SelectedWalls = new ArrayList<Wall>();

        // select wall if within SelectionDistThreshold
        for (Wall w : this.Walls)
        {
            if (IsNearWall(x, y, w))
            {
                // add vertices to SelectedWalls and return false
                this.SelectedWalls.add(w);
                repaint();
                return true;
            }
        }

        // return false if no wall was selected
        repaint();
        return false;
    }

    // Check if a line centered at (x, y) but perpendicular to the wall
    // intersects with the wall
    private static boolean IsNearWall(int x, int y, Wall wall)
    {
        // Makes it a bit easier to read
        float x1, x2, y1, y2;
        x1 = wall.Line.a.x;
        y1 = wall.Line.a.y;
        x2 = wall.Line.b.x;
        y2 = wall.Line.b.y;

        // Get the angle of the orthogonal line
        float angle = (float)Math.atan2(x1 - x2, y2 - y1);

        // Get vertices in the orthogonal line
        Vertex v1 = new Vertex(-1, 
            (float)(x - SelectionDistThreshold * Math.cos(angle)),
            (float)(y - SelectionDistThreshold * Math.sin(angle))
        );
        Vertex v2 = new Vertex(-1, 
            (float)(x + SelectionDistThreshold * Math.cos(angle)),
            (float)(y + SelectionDistThreshold * Math.sin(angle))
        );

        // Create orthogonal line
        Pair<Vertex, Vertex> orthogonalLine = new Pair<Vertex, Vertex>(v1, v2);

        // Do the lines intersect?
        return Intersect(wall.Line, orthogonalLine);
    }

    private static boolean Intersect(Pair<Vertex, Vertex> l1, Pair<Vertex, Vertex> l2)
    {
        if (l1.a == l2.b || l1.b == l2.b || l1.b == l2.a || l1.a == l2.b)
            return false;
        
        return CCW(l1.a, l2.a, l2.b) != CCW(l1.b, l2.a, l2.b) && CCW(l1.a, l1.b, l2.a) != CCW(l1.a, l1.b, l2.b);

    }

    private static boolean CCW(Vertex v1, Vertex v2, Vertex v3)
    {
        return (v3.y - v1.y)*(v2.x - v1.x) > (v2.y - v1.y) * (v3.x - v1.x);
    }
}