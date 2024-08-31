package com.actions;

import com.canvas.Canvas;
import com.extensions.ActionWithDialog;
import com.handlers.FileHandler;
import com.models.MapDat;

import java.awt.event.ActionEvent;
import java.io.File;
import javax.swing.JFileChooser;

public class LoadMap extends ActionWithDialog
{
    private Canvas Canvas;
    private FileHandler<MapDat> FileHandler;
    
    public LoadMap(Canvas canvas)
    {
        this.Canvas = canvas;
        this.FileHandler = new FileHandler<MapDat>();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        try
        {
            String filePath = "";
            JFileChooser fileChooser = new JFileChooser(); 
            int result = fileChooser.showOpenDialog(null); 
            if (result == JFileChooser.APPROVE_OPTION) 
            { 
                File selectedFile = fileChooser.getSelectedFile(); 
                filePath = selectedFile.getAbsolutePath(); // Save the filePath to a string or use it to save the file 
            }

            if (filePath != "")
            {
                MapDat map = (MapDat)FileHandler.ReadObject(filePath);
                InjectToCanvas(map);
                System.out.println("Map loaded from " + filePath);
            }
        }
        catch(Exception ex)
        {
            ShowError("Failed to load map.\n" + ex.getMessage());
        }
    }

    public void InjectToCanvas(MapDat mapDat)
    {
        this.Canvas.Vertices = mapDat.Vertices;
        this.Canvas.Walls = mapDat.Walls;
        this.Canvas.repaint();

    }
}