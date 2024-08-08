import javax.swing.*;
import javax.swing.border.Border;

import java.awt.*;

public class EditorWindow extends JFrame
{
    private Canvas Canvas;
    private Tools Tools;

    EditorWindow()
    {
        this(800, 600, "doomy Editor");
    }
    EditorWindow(int width, int height, String title) 
    {
        this(width, height, title, null);
    }
    EditorWindow(int width, int height, String title, LayoutManager layoutManager) 
    {
        setSize(width, height);
        setTitle(title);
        setLayout(layoutManager);
        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.Canvas = new Canvas();
        this.Tools = new Tools(Canvas);

        add(this.Canvas, BorderLayout.CENTER);
        add(this.Tools, BorderLayout.EAST);

        // add(new Tools(this.Canvas), BorderLayout.EAST);
        // add(new Canvas(), BorderLayout.CENTER);
    }
}