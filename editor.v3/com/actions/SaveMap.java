package com.actions;

import java.awt.event.ActionEvent;
import java.io.File;
import javax.swing.JFileChooser;
import com.canvas.Canvas;
import com.extensions.ActionWithDialog;
import com.handlers.FileHandler;
import com.models.MapDat;

public class SaveMap extends ActionWithDialog
{
    private Canvas Canvas;
    private FileHandler<MapDat> FileHandler;
    public SaveMap(Canvas canvas)
    {
        this.Canvas = canvas;
        this.FileHandler = new FileHandler<MapDat>();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        try
        {
            MapDat map = new MapDat(this.Canvas.Vertices, this.Canvas.Walls);

            String filePath = "";
            JFileChooser fileChooser = new JFileChooser(); 
            int result = fileChooser.showSaveDialog(null); 
            if (result == JFileChooser.APPROVE_OPTION) 
            { 
                File selectedFile = fileChooser.getSelectedFile(); 
                filePath = selectedFile.getAbsolutePath(); // Save the filePath to a string or use it to save the file 
            }

            if (filePath != "")
            {
                this.FileHandler.WriteObject(filePath, map);
                System.out.println("Map saved to " + filePath);
            }
        }
        catch(Exception ex)
        {
            ShowError("Failed to save map.\n" + ex.getMessage());
        }
    }
    
}