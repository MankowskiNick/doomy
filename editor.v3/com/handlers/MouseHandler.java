package com.handlers;
import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import com.canvas.Canvas;
import com.canvas.EditMode;

public class MouseHandler implements MouseListener
{
    private Canvas Canvas;

    public MouseHandler(Canvas canvas)
    {
        this.Canvas = canvas;
    }
    
    @Override
    public void mouseClicked(MouseEvent e) 
    {
        Point canvasLocation = this.Canvas.getLocationOnScreen();
        Point mouseLocation = e.getLocationOnScreen();
        int x = mouseLocation.x - canvasLocation.x;
        int y = mouseLocation.y - canvasLocation.y;
        
        // add vertex if in add mode
        if (this.Canvas.Mode == EditMode.ADD)
            this.Canvas.AddVertex(x, y);

        // select vertex if in edit mode
        else if (this.Canvas.Mode == EditMode.EDIT && !this.Canvas.DragMode)
            this.Canvas.SelectVertex(x, y);

        else if (this.Canvas.Mode == EditMode.EDIT && this.Canvas.DragMode)
        {
            // prevent null pointer exception
            if (this.Canvas.SelectedVertex == null)
                return;

            // move selected vertex
            this.Canvas.SelectedVertex.x = x;
            this.Canvas.SelectedVertex.y = y;
            this.Canvas.repaint();
        }
    }
    
    @Override
    public void mousePressed(MouseEvent e) { }

    @Override
    public void mouseReleased(MouseEvent e) { }

    @Override
    public void mouseEntered(MouseEvent e) { }
    @Override
    public void mouseExited(MouseEvent e) { }
}