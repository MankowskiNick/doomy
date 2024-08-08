import javax.swing.JComponent;

public class WallMenu extends OptionMenu 
{
    WallMenu()
    {
        super("Wall Options");

        this.Components = new JComponent[] 
        {
            new ActionButton("Add Wall"),
            new ActionButton("Edit Wall"),
        };
        this.AttachComponents();
    }
}