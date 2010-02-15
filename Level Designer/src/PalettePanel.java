import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.util.Vector;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;

public class PalettePanel extends JPanel implements Scrollable, ActionListener {
	
	private static final long serialVersionUID = 1L;
	
	private EditorWindow editorWindow;
	
	private BufferedImage activeTile;
	
	private Vector<JRadioButton> paletteSelections;
	
	public PalettePanel(EditorWindow editorWindow) {
		this.editorWindow = editorWindow;
		
		createPaletteChooser();
		
		this.activeTile = EditorPanel.SHEEP;
		this.editorWindow.editorPanel.activeTile = this.activeTile;
		
		update();
	}
	
	public void createPaletteChooser() {
		int numberOfSprites = 4;
		paletteSelections = new Vector<JRadioButton>();
		ButtonGroup paletteSelectionGroup = new ButtonGroup();
		JRadioButton paletteSelection;
		for(int i=0;i<numberOfSprites;i++) {
			paletteSelection = new JRadioButton();
			paletteSelection.addActionListener(this);
			paletteSelectionGroup.add(paletteSelection);
			paletteSelection.setVisible(true);
			paletteSelections.add(paletteSelection);
			this.add(paletteSelection);
		}
		paletteSelections.elementAt(0).setSelected(true);
	}
	
	public void drawSprites(Graphics g) {
		/*Graphics2D g2 = (Graphics2D) g;
		int yPos = 20;
		int xPos = 30;
		int inc = 40;
		g2.drawImage(EditorPanel.SHEEP, null, xPos + 30, yPos += inc);
		g2.drawImage(EditorPanel.LANDMINE, null, xPos + 10, yPos += inc);
		g2.drawImage(EditorPanel.ROCK, null, xPos, yPos += inc);
		yPos += 75;
		g2.drawImage(EditorPanel.FENCE, null, xPos, yPos += inc);
		
		xPos = 0;
		yPos = 20;
		for(int i=0;i<paletteSelections.size();i++) {
			if(i==2) { yPos += 35; }
			else if(i==3) { yPos += 75; }
			paletteSelections.elementAt(i).setLocation(xPos + 5, yPos += inc);
		}*/
	}
	
	public Dimension getPreferredSize() {
		return new Dimension(160, 728);
	}
	
	public Dimension getPreferredScrollableViewportSize() {
		return getPreferredSize();
	}

	public int getScrollableUnitIncrement(Rectangle visibleRect, int orientation, int direction) {
		int currentPosition = 0;
		if(orientation == SwingConstants.HORIZONTAL) {
			currentPosition = visibleRect.x;
		}
		else {
			currentPosition = visibleRect.y;
		}
        
		int maxUnitIncrement = 7;
		if(direction < 0) {
			int newPosition = currentPosition -
							  (currentPosition / maxUnitIncrement)
                              * maxUnitIncrement;
            return (newPosition == 0) ? maxUnitIncrement : newPosition;
        }
		else {
            return ((currentPosition / maxUnitIncrement) + 1)
                   * maxUnitIncrement
                   - currentPosition;
        }
	}
	
	public int getScrollableBlockIncrement(Rectangle visibleRect, int orientation, int direction) {
		if(orientation == SwingConstants.HORIZONTAL) {
			return visibleRect.width - 5;
		}
		else {
			return visibleRect.height - 5;
		}
	}
	
	public boolean getScrollableTracksViewportHeight() {
		return false;
	}

	public boolean getScrollableTracksViewportWidth() {
		return false;
	}
	
	public void actionPerformed(ActionEvent e) {
		for(int i=0;i<paletteSelections.size();i++) {
			if(e.getSource() == paletteSelections.elementAt(i)) {
				if(i == 0) {
					this.activeTile =  EditorPanel.SHEEP;
				}
				else if(i == 1) {
					this.activeTile =  EditorPanel.LANDMINE;
				}
				else if(i == 2) {
					this.activeTile =  EditorPanel.ROCK;
				}
				else if(i == 3) {
					this.activeTile =  EditorPanel.FENCE;
				}
				this.editorWindow.editorPanel.activeTile = this.activeTile;
			}
		}
		this.update();
	}
		
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		
		g.clearRect(0, 0, this.getWidth(), this.getHeight());
		
		drawSprites(g);
	}
	
	public void update() {
		this.repaint();
	}
}
