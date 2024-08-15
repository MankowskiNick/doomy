package com.handlers;
import java.awt.event.ActionEvent;
import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.JComponent;
import javax.swing.KeyStroke;

import com.canvas.Canvas;

public class KeyboardHandler {
    private Canvas Canvas;

    public KeyboardHandler(Canvas canvas) {
        Canvas = canvas;
        setupKeyBindings();
    }

    private void setupKeyBindings() {
        Action toggleDragMode = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Canvas.DragMode = !Canvas.DragMode;
            }
        };

        Action toggleMultiSelectMode = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Canvas.MultiSelectMode = !Canvas.MultiSelectMode;
            }
        };        

        // Get the input map and action map of the canvas
        InputMap inputMap = Canvas.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
        ActionMap actionMap = Canvas.getActionMap();

        // Bind keys to actions
        inputMap.put(KeyStroke.getKeyStroke("shift pressed SHIFT"), "toggleDragMode");
        inputMap.put(KeyStroke.getKeyStroke("released SHIFT"), "toggleDragMode");
        inputMap.put(KeyStroke.getKeyStroke("control pressed CONTROL"), "toggleMultiSelectMode");
        inputMap.put(KeyStroke.getKeyStroke("released CONTROL"), "toggleMultiSelectMode");
        actionMap.put("toggleDragMode", toggleDragMode);
        actionMap.put("toggleMultiSelectMode", toggleMultiSelectMode);
    }
}