import javax.swing.*;
import java.awt.*;
import com.tools.Tools;
import com.canvas.Canvas;
import com.handlers.*;

// TODO: move runnables to actions

public class Editor extends JFrame
{
    public static void main(String[] args) {
        Editor window = new Editor(800, 600, "doomy Editor", new BorderLayout());
    }
    private Canvas Canvas;
    private Tools Tools;
    private KeyboardHandler KeyboardHandler;

    Editor()
    {
        this(800, 600, "doomy Editor");
    }
    Editor(int width, int height, String title) 
    {
        this(width, height, title, null);
    }
    Editor(int width, int height, String title, LayoutManager layoutManager) 
    {
        // Set window properties
        setSize(width, height);
        setTitle(title);
        setLayout(layoutManager);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setVisible(true);

        // Create canvas and tools sidebar
        Canvas = new Canvas();
        Tools = new Tools(Canvas);

        // Add canvas and tools to window
        add(Canvas, BorderLayout.CENTER);
        add(Tools, BorderLayout.EAST);
        // add(new JButton("test"), BorderLayout.CENTER);

        // Add input handlers
        addMouseListener(new MouseHandler(Canvas));
        KeyboardHandler = new KeyboardHandler(Canvas); // TODO: probably move this back to a key listener
        
        revalidate();
        repaint();
    }
}