import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.function.Supplier;

public class Switch extends JToggleButton
{
    Switch()
    {
        this("On", "Off", (Supplier)null, (Supplier)null);
    }

    Switch(String label1, String label2)
    {
        this(label1, label2, (Supplier)null, (Supplier)null);
    }

    Switch(String label1, String label2, Supplier action1, Supplier action2)
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
                        action2.get();
                } 
                else 
                {
                    setText(label1);
                    if (action1 != null)
                        action1.get();
                }
            }
        });
        
    }
}