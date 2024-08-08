import javax.swing.JComponent;

public class SectorMenu extends OptionMenu
{
    SectorMenu()
    {
        super("Sector Options");

        this.Components = new JComponent[] 
        {
            new ActionButton("Process Sectors"),
            new ActionButton("Edit Sector"),
            new ActionButton("Clear Sectors")
        };
        this.AttachComponents();
    }
}