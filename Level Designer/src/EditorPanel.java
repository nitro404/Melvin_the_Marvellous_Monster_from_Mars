import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

public class EditorPanel extends JPanel implements Scrollable, ActionListener, MouseListener, MouseMotionListener {
	
	private static final long serialVersionUID = 1L;
	
	private World world;
	private String spriteDirectory;
	private String spriteSheetFileName;
	
	private Vertex selectedPoint;
	private Vertex selectedVertex;
	private Vertex vertexToMove;
	
	private JPopupMenu popupMenu;
	private JMenuItem popupMenuNewVertex;
	private JMenuItem popupMenuDeleteVertex;
	private JMenuItem popupMenuCancel;
	
	public Sprite activeSprite;
	public static Sprite ALIEN_SPRITESHEET_IMAGE;
	public static SpriteSheet ALIEN_SPRITESHEET;
	public static Sprite ALIEN;
	public static Sprite PLACEHOLDER_SPRITESHEET_IMAGE;
	public static SpriteSheet PLACEHOLDER_SPRITESHEET;
	public static Sprite PLACEHOLDER;
	public static SpriteSheet PLACEHOLDER_SPRITESHEET2;
	
	private Point selectedGridBlock;
	public int mode;
	public boolean gridEnabled;
	final public static int MODE_TILING = 0;
	final public static int MODE_DRAWING = 1;
	final private static int DEFAULT_SELECTION_RADIUS = 6;
	public Color selectedColour;
	public Color gridColour;
	public Color lineColour;
	public Color vertexColour; 
	
	final private int doubleClickSpeed = 200;
	private long lastMouseDown = 0;
	
	public EditorPanel(String spriteDirectory, String spriteSheetFileName, Variables settings) {
		world = null;
		setLayout(null);
		addMouseListener(this);
		addMouseMotionListener(this);
		
		this.spriteDirectory = spriteDirectory;
		this.spriteSheetFileName = spriteSheetFileName;
		
		createPopupMenu();
		
		mode = MODE_TILING;
		gridEnabled = true;
		selectedGridBlock = null;
		activeSprite = null;
		
		selectedPoint = null;
		selectedVertex = null;
		vertexToMove = null;
	
		loadSettings(settings);
		
		loadImages();
		
		update();
	}
	
	private void loadSettings(Variables settings) {
		gridColour = new Color(64, 64, 64);
		lineColour = new Color(0, 0, 0);
		vertexColour = new Color(0, 0, 0);
		selectedColour = new Color(255, 0, 0);
		if(settings != null) {
			Color temp;
			if((temp = Utilities.parseColour(settings.getValue("Grid Colour"))) != null) {
				gridColour = temp;
			}
			if((temp = Utilities.parseColour(settings.getValue("Line Colour"))) != null) {
				lineColour = temp;
			}
			if((temp = Utilities.parseColour(settings.getValue("Vertex Colour"))) != null) {
				vertexColour = temp;
			}
			if((temp = Utilities.parseColour(settings.getValue("Selected Colour"))) != null) {
				selectedColour = temp;
			}
		}
	}
	
	private void createPopupMenu() {
		popupMenu = new JPopupMenu();
		popupMenuNewVertex = new JMenuItem("Create Vertex");
		popupMenuDeleteVertex = new JMenuItem("Delete Vertex");
		popupMenuCancel = new JMenuItem("Cancel");
		
		popupMenuNewVertex.addActionListener(this);
		popupMenuDeleteVertex.addActionListener(this);
		
		popupMenu.add(popupMenuNewVertex);
		popupMenu.add(popupMenuDeleteVertex);
		popupMenu.addSeparator();
		popupMenu.add(popupMenuCancel);
	}
	
	public void loadImages() {
		try {
			SpriteSheets spriteSheets = SpriteSheets.parseFrom(spriteSheetFileName, spriteDirectory);
			ALIEN_SPRITESHEET = spriteSheets.getSpriteSheet("Alien");
			PLACEHOLDER_SPRITESHEET = spriteSheets.getSpriteSheet("Placeholder 1");  
			PLACEHOLDER_SPRITESHEET2 = spriteSheets.getSpriteSheet("Placeholder 2");
			/*
			ALIEN_SPRITESHEET_IMAGE = new Sprite("Alien.png", spriteDirectory);
			ALIEN_SPRITESHEET = new SpriteSheet(ALIEN_SPRITESHEET_IMAGE, "Alien", 1, 1, 126, 126, 128, 128, true, 8, 8);
			ALIEN = ALIEN_SPRITESHEET.getSprite(0);
			PLACEHOLDER_SPRITESHEET_IMAGE = new Sprite("Placeholder1.png", spriteDirectory);
			PLACEHOLDER_SPRITESHEET = new SpriteSheet(PLACEHOLDER_SPRITESHEET_IMAGE, "Placeholder 1", 1, 1, 64, 64, 66, 66, true, 1, 2);
			PLACEHOLDER = PLACEHOLDER_SPRITESHEET.getSprite(0);
			*/
			ALIEN = ALIEN_SPRITESHEET.getSprite(0);
			PLACEHOLDER = PLACEHOLDER_SPRITESHEET.getSprite(0);
			
			PLACEHOLDER = PLACEHOLDER_SPRITESHEET2.getSprite("Christmas Tree");
		}
		catch(Exception e) { }
	}
	
	public void setWorld(World world) {
		this.world = world;
	}
	
	public Dimension getPreferredSize() {
		if(world != null) {
			return world.dimensions;
		}
		else {
			return new Dimension(16 * World.GRID_SIZE, 16 * World.GRID_SIZE);
		}
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
			return visibleRect.width - World.GRID_SIZE;
		}
		else {
			return visibleRect.height - World.GRID_SIZE;
		}
	}
	
	public boolean getScrollableTracksViewportHeight() {
		return false;
	}

	public boolean getScrollableTracksViewportWidth() {
		return false;
	}
	
	public void mouseClicked(MouseEvent e) { }
	public void mouseEntered(MouseEvent e) { }
	public void mouseExited(MouseEvent e) { }
	
	public void mousePressed(MouseEvent e) {
		vertexToMove = null;
		if(e.getButton() == MouseEvent.BUTTON1) {
			if(e.getWhen() - lastMouseDown < doubleClickSpeed) {
				world.addVertex(new Vertex(e.getPoint()));
			}
		}
		else if(e.getButton() == MouseEvent.BUTTON2) {
			Vertex previousVertex = selectedVertex;
			Vertex previousPoint = selectedPoint;
			
			selectedPoint = new Vertex(e.getPoint());
			selectVertex(e.getPoint(), DEFAULT_SELECTION_RADIUS);
			vertexToMove = selectedVertex; 
			
			selectedVertex = previousVertex;
			previousVertex = previousPoint;
		}
		lastMouseDown = e.getWhen();
	}
	
	public void mouseReleased(MouseEvent e) {
		if(mode == MODE_DRAWING) {
			if(e.getButton() == MouseEvent.BUTTON3) {
				selectedPoint = new Vertex(e.getPoint());
				selectVertex(e.getPoint(), DEFAULT_SELECTION_RADIUS);
				popupMenuDeleteVertex.setEnabled(selectedVertex != null);
				popupMenu.show(this, e.getX(), e.getY());
			}
			else if(e.getButton() == MouseEvent.BUTTON1) {
				Vertex previousVertex = null;
				if(selectedVertex != null) {
					previousVertex = selectedVertex; 
				}
				selectedPoint = new Vertex(e.getPoint());
				selectVertex(e.getPoint(), DEFAULT_SELECTION_RADIUS);
				
				if(previousVertex != null && selectedVertex != null && !previousVertex.equals(selectedVertex)) {
					Edge newEdge = new Edge(previousVertex, selectedVertex);
					
					if(this.world.edges.containsEdge(newEdge) ||
					   this.world.edges.containsEdge(new Edge(selectedVertex, previousVertex))) {
						return;
					}
					
					int result = JOptionPane.showConfirmDialog(this, "Create edge?", "Edge Creation", JOptionPane.YES_NO_OPTION);
					if(result == JOptionPane.YES_OPTION) {
						this.world.addEdge(newEdge);
					}
				}
			}
		}
		else if(mode == MODE_TILING) {
			if(e.getButton() == MouseEvent.BUTTON1) {
				if(selectedGridBlock != null && activeSprite == ALIEN) {
					world.objects.add(new Entity(new Vertex(selectedGridBlock.x, selectedGridBlock.y), 0, 0));
				}
			}
		}
		this.update();
	}
	public void mouseDragged(MouseEvent e) {
		if(mode == MODE_DRAWING) {
			if(vertexToMove != null) {
				vertexToMove.x = e.getPoint().x;
				vertexToMove.y = e.getPoint().y;
			}
		}
		this.repaint();
	}
	
	public void mouseMoved(MouseEvent e) {
		if(mode == MODE_TILING) {
			getSelectedGridBlock(e.getPoint());
		}
		this.repaint();
	}
	
	public void actionPerformed(ActionEvent e) {
		if(world != null && mode == MODE_DRAWING) {
			if(e.getSource().equals(popupMenuNewVertex)) {
				world.addVertex(new Vertex(selectedPoint.x, selectedPoint.y));
				selectedVertex = null;
			}
			else if(e.getSource().equals(popupMenuDeleteVertex)) {
				world.removeVertex(selectedVertex);
				selectedVertex = null;
			}
		}
		this.update();
	}
	
	public void selectVertex(Point p, int r) {
		if(p == null) { return; }
		if(r < 0) { r = 6; }
		selectedVertex = null;
		if(world != null) {
			for(int i=0;i<world.edges.vertices.size();i++) {
				Vertex v = world.edges.vertices.elementAt(i);
				if(Math.sqrt( Math.pow( (selectedPoint.x - v.x) , 2) + Math.pow( (selectedPoint.y - v.y) , 2) ) <= r) {
					selectedVertex = v;
				}
			}
		}
		update();
	}
	
	public void getSelectedGridBlock(Point p) {
		Point current = p;
		Point offset = new Point(current.x, current.y);
		Point location = new Point(offset.x / World.GRID_SIZE,
				 				   offset.y / World.GRID_SIZE);
		if(location.x < 0 || location.y < 0 || location.x >= world.gridSize.x || location.y >= world.gridSize.y) {
			selectedGridBlock = null;
		}
		selectedGridBlock = location;
	}
	
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		
		if(world != null) {
			g.clearRect(0, 0, this.getWidth(), this.getHeight());
			world.paintOn(g, lineColour, vertexColour);
		}
		
		drawObjects(g);
		
		drawGrid(g);
		
		if(mode == MODE_TILING && selectedGridBlock != null && activeSprite != null) {
			activeSprite.paintOn(g, selectedGridBlock.x * World.GRID_SIZE, selectedGridBlock.y * World.GRID_SIZE);
		}
	}
	
	public void drawGrid(Graphics g) {
		if(world != null) {
			g.setColor(gridColour);
			
			if(gridEnabled) {
				for(int i=0;i<world.gridSize.x+1;i++) {
					g.drawLine(i * World.GRID_SIZE, 0, i * World.GRID_SIZE, world.gridSize.y * World.GRID_SIZE);
				}
				
				for(int j=0;j<world.gridSize.y+1;j++) {
					g.drawLine(0, j * World.GRID_SIZE, world.gridSize.x * World.GRID_SIZE, j * World.GRID_SIZE);
				}
				
				if(mode == MODE_TILING && selectedGridBlock != null) {
					Graphics2D g2 = (Graphics2D) g;
					Stroke s = g2.getStroke();
					g2.setStroke(new BasicStroke(2));
					g2.setColor(selectedColour);
					int w = World.GRID_SIZE;
					int x = selectedGridBlock.x;
					int y = selectedGridBlock.y;
					g2.drawLine( x*w,     y*w,   (x*w)+w,  y*w);
					g2.drawLine((x*w)+w,  y*w,   (x*w)+w, (y*w)+w);
					g2.drawLine((x*w)+w, (y*w)+w, x*w,    (y*w)+w);
					g2.drawLine( x*w,    (y*w)+w, x*w,     y*w);
					g2.setStroke(s);
				}
			}
			else {
				g.drawLine(0, 0, 0, world.gridSize.y * World.GRID_SIZE);
				g.drawLine(world.gridSize.x * World.GRID_SIZE, 0, world.gridSize.x * World.GRID_SIZE, world.gridSize.y * World.GRID_SIZE);
				g.drawLine(0, 0, world.gridSize.x * World.GRID_SIZE, 0);
				g.drawLine(0, world.gridSize.y * World.GRID_SIZE, world.gridSize.x * World.GRID_SIZE, world.gridSize.y * World.GRID_SIZE);
			}
		}
		
		g.setColor(selectedColour);
		if(mode == MODE_DRAWING) {
			if(vertexToMove != null) {
				vertexToMove.paintOn(g);
			}
			else if(selectedVertex != null) {
				selectedVertex.paintOn(g);
			}
		}
	}
	
	public void drawObjects(Graphics g) {
		for(int i=0;i<world.objects.size();i++) {
			activeSprite.paintOn(g, world.objects.elementAt(i).location.x * World.GRID_SIZE, world.objects.elementAt(i).location.y * World.GRID_SIZE); 
		}
	}
	
	public void update() {
		this.repaint();
	}
}
