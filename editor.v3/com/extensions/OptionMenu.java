package com.extensions;
import javax.swing.*;

public class OptionMenu extends JPanel
{
    protected JComponent[] Components;

    public OptionMenu(String title) 
    {
        setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        setBorder(BorderFactory.createTitledBorder(title));
        setVisible(true);
    }

    public void AttachComponents()
    {
        for (JComponent c : this.Components)
        {
            add(c);
        }
        revalidate();
        repaint();
    }
}