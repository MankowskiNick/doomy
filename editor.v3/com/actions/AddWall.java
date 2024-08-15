package com.actions;

import java.awt.event.ActionEvent;
import javax.swing.*;
import java.awt.*;
import com.canvas.Canvas;
import com.extensions.ActionWithDialog;
import com.models.*;

public class AddWall extends ActionWithDialog
{
    private Canvas Canvas;
    public AddWall(Canvas canvas)
    {
        super();
        this.Canvas = canvas;
    }
    @Override
    public void actionPerformed(ActionEvent e) 
    {
        // Add wall
        if (Canvas.SelectedVertices.size() == 2)
        {
            Wall wall = CreateWallFromDialog();
            if (wall != null)
                this.Canvas.Walls.add(wall);
            this.Canvas.SelectedVertices.clear();
            this.Canvas.repaint();
        }
        else
        {
            ShowError("Select 2 vertices to create a wall.");
        }
    }

    private Wall CreateWallFromDialog() 
    {   
        try
        {
           // Create dialog
            JDialog dialog = new JDialog();
            dialog.setTitle("Create Wall");
            dialog.setLayout(new GridLayout(9, 2));
        
            // RGB values
            JLabel rLabel = new JLabel("R:");
            JTextField rField = new JTextField();
            JLabel gLabel = new JLabel("G:");
            JTextField gField = new JTextField();
            JLabel bLabel = new JLabel("B:");
            JTextField bField = new JTextField();
        
            // Heights
            JLabel floorHeightLabel = new JLabel("Floor Height:");
            JTextField floorHeightField = new JTextField();
            JLabel ceilingHeightLabel = new JLabel("Ceiling Height:");
            JTextField ceilingHeightField = new JTextField();
            JLabel minHeightLabel = new JLabel("Min Height:");
            JTextField minHeightField = new JTextField();
            JLabel maxHeightLabel = new JLabel("Max Height:");
            JTextField maxHeightField = new JTextField();
        
            // Wall type
            JLabel wallTypeLabel = new JLabel("Wall Type:");
            JComboBox<Wall.WallType> wallTypeComboBox = new JComboBox<>(Wall.WallType.values());
        
            // Add components to dialog
            dialog.add(rLabel);
            dialog.add(rField);
            dialog.add(gLabel);
            dialog.add(gField);
            dialog.add(bLabel);
            dialog.add(bField);
            dialog.add(floorHeightLabel);
            dialog.add(floorHeightField);
            dialog.add(ceilingHeightLabel);
            dialog.add(ceilingHeightField);
            dialog.add(minHeightLabel);
            dialog.add(minHeightField);
            dialog.add(maxHeightLabel);
            dialog.add(maxHeightField);
            dialog.add(wallTypeLabel);
            dialog.add(wallTypeComboBox);
        
            // OK button
            JButton okButton = new JButton("OK");
            okButton.addActionListener(event -> dialog.dispose());
            dialog.add(okButton);
        
            dialog.pack();
            dialog.setModal(true);
            dialog.setVisible(true);
        
            // Retrieve values
            int r = Integer.parseInt(rField.getText());
            int g = Integer.parseInt(gField.getText());
            int b = Integer.parseInt(bField.getText());
            int floorHeight = Integer.parseInt(floorHeightField.getText());
            int ceilingHeight = Integer.parseInt(ceilingHeightField.getText());
            int minHeight = Integer.parseInt(minHeightField.getText());
            int maxHeight = Integer.parseInt(maxHeightField.getText());
            Wall.WallType wallType = (Wall.WallType) wallTypeComboBox.getSelectedItem();
        
            // Create and return Wall object
            Pair<Vertex, Vertex> line = new Pair<>(
                this.Canvas.SelectedVertices.get(0), 
                this.Canvas.SelectedVertices.get(1)
            );
            Wall wall = new Wall(
                this.Canvas.NextWallId++, 
                line, 
                new int[] {r, g, b}, 
                wallType,
                floorHeight, ceilingHeight, 
                minHeight, maxHeight
            );
            return wall;
        } 
        catch (Exception e) 
        {
            ShowError("Invalid input. Please enter valid values.");
            return null;
        }
    }
}