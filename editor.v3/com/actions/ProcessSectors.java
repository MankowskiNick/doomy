package com.actions;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;

import com.canvas.Canvas;
import com.extensions.ActionWithDialog;
import com.models.BSPTree;
import com.models.MapDat;

public class ProcessSectors extends ActionWithDialog
{
    private Canvas Canvas;

    public ProcessSectors(Canvas canvas)
    {
        super();
        Canvas = canvas;
    }

    @Override
    public void actionPerformed(ActionEvent e)
    {
        this.GetConfirmationPopup();
    }

    private void GetConfirmationPopup()
    {
        JDialog dialog = new JDialog();
        dialog.setTitle("Confirmation");
        dialog.setLayout(new BorderLayout());

        JLabel label = new JLabel("Confirm process sectors?");
        JButton confirmButton = new JButton("Confirm");
        JButton cancelButton = new JButton("Cancel");

        dialog.add(label, BorderLayout.NORTH);
        dialog.add(confirmButton, BorderLayout.EAST);
        dialog.add(cancelButton, BorderLayout.WEST);
        dialog.pack();
        dialog.setVisible(true);

        confirmButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                dialog.dispose(); // Close the dialog
                BuildBSP();
            }
        });

        cancelButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                dialog.dispose(); // Close the dialog
            }
        });
    }

    private void BuildBSP()
    {
        MapDat map = new MapDat(this.Canvas.Vertices, this.Canvas.Walls);
        BSPTree tree = new BSPTree(map);
        this.Canvas.SetTree(tree);
        this.Canvas.repaint();
        System.out.println("BSP Tree constructed.");
    }
}