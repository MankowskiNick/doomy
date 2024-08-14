import javax.swing.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class KeyboardInputExample extends JFrame implements KeyListener {

    private JTextArea textArea;

    public KeyboardInputExample() {
        setTitle("Keyboard Input Example");
        setSize(400, 300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        textArea = new JTextArea();
        textArea.addKeyListener(this);

        add(new JScrollPane(textArea));
        setVisible(true);
    }

    @Override
    public void keyPressed(KeyEvent e) {
        textArea.append("Key Pressed: " + KeyEvent.getKeyText(e.getKeyCode()) + "\n");
    }

    @Override
    public void keyReleased(KeyEvent e) {
        textArea.append("Key Released: " + KeyEvent.getKeyText(e.getKeyCode()) + "\n");
    }

    @Override
    public void keyTyped(KeyEvent e) {
        textArea.append("Key Typed: " + e.getKeyChar() + "\n");
    }

    public static void main(String[] args) {
        new KeyboardInputExample();
    }
}
