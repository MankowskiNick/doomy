import javax.swing.*;

public class OptionMenu extends JPanel
{
    JComponent[] Components;

    OptionMenu(String title) 
    {
        setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        setBorder(BorderFactory.createTitledBorder(title));
    }

    public void AttachComponents()
    {
        for (JComponent c : this.Components)
        {
            add(c);
        }
    }
}