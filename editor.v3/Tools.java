import javax.swing.*;

public class Tools extends OptionMenu
{
    private Canvas Canvas;

    Tools(Canvas canvas)
    {
        this(200, 600, canvas);
    }

    Tools(int width, int height, Canvas canvas)
    {
        super("Tools");
        setSize(width, height);

        this.Canvas = canvas;

        this.Components = new JComponent[]
        {
            new FileMenu(this.Canvas),
            new WallMenu(),
            new SectorMenu()
        };
        this.AttachComponents();
        setFocusable(false);
    }
}

// chapter 35 thru va(?)
// war orphans of ohio