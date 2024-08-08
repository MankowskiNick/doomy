
import java.awt.event.ActionEvent;
import java.beans.PropertyChangeListener;

import javax.swing.*;

public class FileMenu extends OptionMenu
{
    FileMenu(Canvas canvas)
    {
        super("File");

        this.Components = new JComponent[] 
        {
            new JLabel("Mode"),
            new Switch("Add", "Edit", () -> canvas.Mode = EditMode.ADD, () -> canvas.Mode = EditMode.EDIT),
            new ActionButton("New Map"),
            new ActionButton("Save Map"),
            new ActionButton("Load Map"),
            new ActionButton("Export Map")
        };

        this.AttachComponents();
    }
}