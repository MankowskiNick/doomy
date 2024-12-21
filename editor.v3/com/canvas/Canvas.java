package com.canvas;
import javax.swing.*;

import com.models.*;
import com.models.BSPTree.BSPNode;

import java.awt.*;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashSet;
import java.util.Random;
import java.util.Set;
import java.lang.Math;


public class Canvas extends JPanel
{
    public ArrayList<Vertex> Vertices = new ArrayList<Vertex>();
    public ArrayList<Wall> Walls = new ArrayList<Wall>();
    private BSPTree Tree;

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

        this.PaintBSPTree(g2d);

        // draw walls
        for (Wall w : this.Walls)
        {
            g2d.setColor(new Color(w.Color[0], w.Color[1], w.Color[2]));
            g2d.setStroke(new BasicStroke(LineThickness));
            g2d.drawLine(
                (int)w.Line.a.x, (int)w.Line.a.y,
                (int)w.Line.b.x, (int)w.Line.b.y
            );
            
            // Draw wall id on the wall
            g2d.setColor(Color.BLACK);
            g2d.drawString(
                Integer.toString(w.Id),
                (int)(w.Line.a.x + w.Line.b.x) / 2,
                (int)(w.Line.a.y + w.Line.b.y) / 2
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

            // Draw vertex id on the vertex
            g2d.setColor(Color.GRAY);
            g2d.drawString(
                Integer.toString(v.id),
                (int)v.x - 3,
                (int)v.y + 3
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

    public void PaintBSPTree(Graphics2D g2d) 
    {

        if (this.Tree == null)
            return;

        for (BSPNode subsect : this.Tree.Subsectors)
        {
            Polygon subsectPolygon = new Polygon();
            // ArrayList<Wall> sortedWalls = subsect.Walls;//this.SortWalls(subsect.Walls);
            ArrayList<Pair<Integer, Integer>> sortedVertices = this.SortVertices(subsect.Walls);
            for (Pair<Integer, Integer> point : sortedVertices)
            {
                subsectPolygon.addPoint(point.a, point.b);
            }
            Random rand = new Random();
            g2d.setColor(
                new Color(
                    64 + rand.nextInt(192 - 64 + 1),
                    64 + rand.nextInt(192 - 64 + 1),
                    64 + rand.nextInt(192 - 64 + 1)
                )
            );
            g2d.setStroke(new BasicStroke());
            g2d.fill(subsectPolygon);
        }
    }

    private ArrayList<Pair<Integer, Integer>> SortVertices(ArrayList<Wall> walls) 
    {
        // get all unique vertices
        Set<Vertex> uniqueVertices = new HashSet<>();
        for (Wall wall : walls) {
            uniqueVertices.add(wall.Line.a);
            uniqueVertices.add(wall.Line.b);
        }

        // get the midpoint of all vertices
        double sumX = 0, sumY = 0;
        int count = uniqueVertices.size();
        for (Vertex v : uniqueVertices) {
            sumX += v.x;
            sumY += v.y;
        }
        Pair<Integer, Integer> middle = new Pair<Integer, Integer>(
            (int)(sumX / count), 
            (int)(sumY / count));

        // order vertices in clockwise order
        ArrayList<Pair<Integer, Integer>> vertexPairs = new ArrayList<Pair<Integer, Integer>>();
        for (Vertex v : uniqueVertices) {
            vertexPairs.add(new Pair<Integer, Integer>((int)v.x, (int)v.y));
        }

        vertexPairs.sort(new Comparator<Pair<Integer, Integer>>() {
            @Override
            public int compare(Pair<Integer, Integer> v1, Pair<Integer, Integer> v2) {
                double angle1 = Math.atan2(v1.b - middle.b, v1.a - middle.a);
                double angle2 = Math.atan2(v2.b - middle.b, v2.a - middle.a);
                return Double.compare(angle1, angle2);
            }
        });

        return new ArrayList<Pair<Integer, Integer>>(vertexPairs);
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

    public void SetTree(BSPTree tree) 
    {
        this.Tree = tree;
        this.Walls = tree.Map.Walls;
        this.Vertices = tree.Map.Vertices;
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