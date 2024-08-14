package com.tools;
import javax.swing.*;

public class ToolsTrayUI {
    public static void main(String[] args) {
        JFrame frame = new JFrame("JComboBox Test");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(400, 200);

        JPanel panel = new JPanel();
        panel.add(new JLabel("Mode"));
        panel.add(new JComboBox<String>(new String[] {"Edit", "Add"}));
        panel.add(new JButton("test"));

        frame.add(panel);
        frame.setVisible(true);
    }
}
