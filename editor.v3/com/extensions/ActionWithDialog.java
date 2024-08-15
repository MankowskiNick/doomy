package com.extensions;

import javax.swing.*;
import java.awt.*;

public abstract class ActionWithDialog extends AbstractAction 
{
    protected void ShowError(String message)
    {
            // Create dialog
            JDialog dialog = new JDialog();

            // Create label and add to dialog
            JLabel label = new JLabel(message);
            label.setHorizontalAlignment(SwingConstants.CENTER);
            dialog.add(label, BorderLayout.CENTER);

            // Create OK button and add to dialog
            JButton okButton = new JButton("OK");
            okButton.addActionListener(event -> dialog.dispose());
            dialog.add(okButton, BorderLayout.SOUTH);

            // Set dialog properties
            dialog.setSize(400, 200);
            dialog.setVisible(true);
    }
}