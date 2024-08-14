package com.extensions;

import javax.swing.*;

public class ActionButton extends JButton
{
    public ActionButton()
    {
        super();
    }

    public ActionButton(String title)
    {
        this(title, (Action)null, (Icon)null);
    }

    public ActionButton(String title, Action a)
    {
        this(title, a, (Icon)null);
    }

    public ActionButton(String title, Action a, Icon i)
    {
        super(title, i);
        setAction(a);
        setText(title);
    }
}