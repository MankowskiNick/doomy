package com.actions;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JTextField;

import com.extensions.ActionWithDialog;
import com.models.*;
import com.canvas.Canvas;

public class EditWall extends ActionWithDialog
{
    private Canvas Canvas;

    public EditWall(Canvas canvas)
    {
        super();
        Canvas = canvas;
    }

    @Override
    public void actionPerformed(ActionEvent e) 
    {
        if (Canvas.SelectedWalls.size() > 0)
        {
            // edit walls
            WallAttributes attribs = GetWallAttribsFromDialog();

            for (Wall w : this.Canvas.SelectedWalls)
            {
                w.Color = attribs.Color;
                w.CeilingHeight = attribs.CeilingHeight;
                w.FloorHeight = attribs.FloorHeight;
                w.MinHeight = attribs.MinHeight;
                w.MaxHeight = attribs.MaxHeight;
                w.Type = attribs.Type;
            }
            Canvas.repaint();
        }
        else
        {
            ShowError("Select at least one wall to edit.");
        }
    }

    private WallAttributes GetWallAttribsFromDialog() 
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
            JComboBox<WallType> wallTypeComboBox = new JComboBox<>(WallType.values());
        
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
            WallType wallType = (WallType) wallTypeComboBox.getSelectedItem();
        
            WallAttributes attribs = new WallAttributes(
                new int[] {r, g, b},
                minHeight,
                maxHeight,
                floorHeight,
                ceilingHeight,
                wallType
            );
            return attribs;
        } 
        catch (Exception e) 
        {
            ShowError("Invalid input. Please enter valid values.");
            return null;
        }
    }

    // Container for wall attributes that can be mass applied(not id or vertices)
    private class WallAttributes
    {
        int[] Color;

        public float MinHeight;
        public float MaxHeight;

        public float FloorHeight;
        public float CeilingHeight;

        public WallType Type;

        public WallAttributes(
            int[] color,
            float minHeight,
            float maxHeight,
            float floorHeight,
            float ceilingHeight,
            WallType type
        )
        {
            this.Color = color;
            this.MinHeight = minHeight;
            this.MaxHeight = maxHeight;
            this.FloorHeight = floorHeight;
            this.CeilingHeight = ceilingHeight;
            this.Type = type;
        }
    }
    
}