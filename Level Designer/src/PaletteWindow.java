import java.io.*;
import java.awt.event.*;

import javax.swing.*;

public class PaletteWindow extends JFrame implements ActionListener {
	
	private EditorWindow editorWindow;
	
	private PalettePanel palettePanel;
	private JScrollPane palettePanelScrollPane;
	
	public PaletteWindow(EditorWindow editorWindow) {
		super("Palette");
		setSize(200, 768);
		setLocation(editorWindow.getX() + editorWindow.getWidth(), editorWindow.getY());
		
		this.editorWindow = editorWindow;
		
		palettePanel = new PalettePanel(editorWindow);
		palettePanelScrollPane = new JScrollPane(palettePanel);
		add(palettePanelScrollPane);
		
		update();
	}
	
	public void actionPerformed(ActionEvent e) { update(); }
	

	public void update() {
		this.repaint();
		palettePanelScrollPane.repaint();
		palettePanel.update();
	}
	
}
