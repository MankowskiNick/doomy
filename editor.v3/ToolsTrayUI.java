// import javax.swing.*;
// import java.awt.*;

// public class ToolsTrayUI {

//     public static void main(String[] args) {
//         // Create the main frame
//         JFrame frame = new JFrame("Tools Tray");
//         frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
//         frame.setSize(400, 600);
//         frame.setLayout(new BorderLayout());

//         // Create the tools panel
//         JPanel toolsPanel = new JPanel();
//         toolsPanel.setLayout(new BoxLayout(toolsPanel, BoxLayout.Y_AXIS));
//         toolsPanel.setBorder(BorderFactory.createTitledBorder("Tools"));

//         // Editor Options panel
//         JPanel editorOptionsPanel = new JPanel();
//         editorOptionsPanel.setLayout(new BoxLayout(editorOptionsPanel, BoxLayout.Y_AXIS));
//         editorOptionsPanel.setBorder(BorderFactory.createTitledBorder("Editor Options"));

//         JComboBox<String> modeComboBox = new JComboBox<>(new String[]{"Edit", "Add"});
//         JButton addButton = new JButton("Add");
//         JButton newMapButton = new JButton("New Map");
//         JButton saveMapButton = new JButton("Save Map");
//         JButton loadMapButton = new JButton("Load Map");
//         JButton exportMapButton = new JButton("Export Map");

//         editorOptionsPanel.add(new JLabel("Mode"));
//         editorOptionsPanel.add(modeComboBox);
//         editorOptionsPanel.add(addButton);
//         editorOptionsPanel.add(newMapButton);
//         editorOptionsPanel.add(saveMapButton);
//         editorOptionsPanel.add(loadMapButton);
//         editorOptionsPanel.add(exportMapButton);

//         // Wall Options panel
//         JPanel wallOptionsPanel = new JPanel();
//         wallOptionsPanel.setLayout(new BoxLayout(wallOptionsPanel, BoxLayout.Y_AXIS));
//         wallOptionsPanel.setBorder(BorderFactory.createTitledBorder("Wall Options"));

//         JButton addWallButton = new JButton("Add Wall");
//         JButton editWallButton = new JButton("Edit Wall");

//         wallOptionsPanel.add(addWallButton);
//         wallOptionsPanel.add(editWallButton);

//         // Sector Options panel
//         JPanel sectorOptionsPanel = new JPanel();
//         sectorOptionsPanel.setLayout(new BoxLayout(sectorOptionsPanel, BoxLayout.Y_AXIS));
//         sectorOptionsPanel.setBorder(BorderFactory.createTitledBorder("Sector Options"));

//         JButton processSectorsButton = new JButton("Process Sectors");
//         JButton editSectorButton = new JButton("Edit Sector");
//         JButton clearSectorsButton = new JButton("Clear Sectors");

//         sectorOptionsPanel.add(processSectorsButton);
//         sectorOptionsPanel.add(editSectorButton);
//         sectorOptionsPanel.add(clearSectorsButton);

//         // Add all option panels to the tools panel
//         toolsPanel.add(editorOptionsPanel);
//         toolsPanel.add(wallOptionsPanel);
//         toolsPanel.add(sectorOptionsPanel);

//         // Add the tools panel to the frame
//         frame.add(toolsPanel, BorderLayout.EAST);

//         // Display the frame
//         frame.setVisible(true);
//     }
// }

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
