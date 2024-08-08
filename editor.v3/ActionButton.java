
import javax.swing.*;

public class ActionButton extends JButton
{
    ActionButton()
    {
        super();
    }

    ActionButton(String title)
    {
        this(title, (Action)null, (Icon)null);
    }

    ActionButton(String title, Action a)
    {
        this(title, a, (Icon)null);
    }

    ActionButton(String title, Action a, Icon i)
    {
        super(title, i);
        setAction(a);
        setText(title);
    }
}