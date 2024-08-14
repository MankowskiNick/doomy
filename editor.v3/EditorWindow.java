// import javax.swing.*;

// import com.canvas.Canvas;
// import com.handlers.KeyboardHandler;
// import com.handlers.MouseHandler;
// import com.tools.Tools;

// import java.awt.*;

// public class EditorWindow extends JFrame
// {
//     private Canvas Canvas;
//     private Tools Tools;
//     private KeyboardHandler KeyboardHandler;

//     EditorWindow()
//     {
//         this(800, 600, "doomy Editor");
//     }
//     EditorWindow(int width, int height, String title) 
//     {
//         this(width, height, title, null);
//     }
//     EditorWindow(int width, int height, String title, LayoutManager layoutManager) 
//     {
//         // Set window properties
//         setSize(width, height);
//         setTitle(title);
//         setLayout(layoutManager);
//         setVisible(true);
//         setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
//         setVisible(true);

//         // Create canvas and tools sidebar
//         Canvas = new Canvas();
//         Tools = new Tools(Canvas);

//         // Add canvas and tools to window
//         add(Canvas, BorderLayout.CENTER);
//         add(Tools, BorderLayout.EAST);

//         // Add input handlers
//         addMouseListener(new MouseHandler(Canvas));
//         KeyboardHandler = new KeyboardHandler(Canvas); // TODO: probably move this back to a key listener
//     }
// }