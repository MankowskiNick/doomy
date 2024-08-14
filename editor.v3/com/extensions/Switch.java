package com.extensions;
import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Switch extends JToggleButton
{
    public Switch()
    {
        this("On", "Off", null, null);
    }

    public Switch(String label1, String label2)
    {
        this(label1, label2, null, null);
    }

    public Switch(String label1, String label2, Runnable action1, Runnable action2)
    {
        super();
        setText(label1);

        addActionListener(new ActionListener() 
        {
            @Override
            public void actionPerformed(ActionEvent e) 
            {
                if (isSelected()) 
                {
                    setText(label2);
                    if (action2 != null)
                        action2.run();
                } 
                else 
                {
                    setText(label1);
                    if (action1 != null)
                        action1.run();
                }
            }
        });
        
    }
}