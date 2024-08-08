import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
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

    Canvas()
    {
        this(600, 600);
    }

    Canvas(int width, int height)
    {
        setSize(width, height);
        setBackground(Color.WHITE);

        // add mouse listener
        this.addMouseListener(new MouseListener() 
        {
            @Override
            public void mouseClicked(MouseEvent e) 
            {
                // add vertex if in add mode
                if (Canvas.this.Mode == EditMode.ADD)
                    Canvas.this.AddVertex(e.getX(), e.getY());

                // select vertex if in edit mode
                else if (Canvas.this.Mode == EditMode.EDIT)
                    Canvas.this.SelectVertex(e.getX(), e.getY());
            }
            
            // the rest of these are not used, but are required
            @Override
            public void mousePressed(MouseEvent e) { }
            @Override
            public void mouseReleased(MouseEvent e) { }
            @Override
            public void mouseEntered(MouseEvent e) { }
            @Override
            public void mouseExited(MouseEvent e) { }
        });
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

    private void AddVertex(int x, int y)
    {
        this.Vertices.add(new Vertex(this.NextVertexId, x, y));
        this.NextVertexId++;
        repaint();
    }

    private void SelectVertex(int x, int y)
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