import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.lang.Math;

public class Canvas extends JPanel
{
    public ArrayList<Vertex> Vertices = new ArrayList<Vertex>();
    public ArrayList<Wall> Walls = new ArrayList<Wall>();

    public EditMode Mode = EditMode.ADD;

    private int NextVertexId = 0;
    private int NextWallId = 0;

    public Vertex SelectedVertex = null;

    private final float SelectionDistThreshold = 6;
    private final int VertexDrawRadius = 3;

    public boolean Dragging = false;
    public boolean DragMode = false;

    Canvas()
    {
        this(600, 600);
    }

    Canvas(int width, int height)
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
            
        }

        // draw outline around SelectedVertex
        if (this.SelectedVertex != null)
        {
            Vertex v = this.SelectedVertex;
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
        boolean found = false;
        for (Vertex v : this.Vertices)
        {
            double distTo = Math.pow(Math.pow(x - v.x, 2) + Math.pow(y - v.y, 2), 0.5);
            if (distTo < this.SelectionDistThreshold)
            {
                found = true;
                this.SelectedVertex = v;
            }
        }
        if (!found)
            this.SelectedVertex = null;

        repaint();
    }
}