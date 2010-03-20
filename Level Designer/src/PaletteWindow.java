import java.awt.event.*;

import javax.swing.*;

public class PaletteWindow extends JFrame implements ActionListener {
	
	private static final long serialVersionUID = 1L;

	private PalettePanel palettePanel;
	private JScrollPane palettePanelScrollPane;
	
	public PaletteWindow(EditorWindow editorWindow) {
		super("Palette");
		setSize(200, 768);
		setLocation(editorWindow.getX() + editorWindow.getWidth(), editorWindow.getY());
		
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
