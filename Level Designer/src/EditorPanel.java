import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

public class EditorPanel extends JPanel implements Scrollable, ActionListener, MouseListener, MouseMotionListener {
	
	private static final long serialVersionUID = 1L;
	
	private World world;
	
	private EditorWindow editorWindow;
	
	private Point selectedPoint;
	private Vertex selectedVertex;
	private Vertex vertexToMove;
	private Vertex lastSelectedVertex;
	private Entity selectedSprite;
	private Entity spriteToMove;
	
	private JPopupMenu drawingPopupMenu;
	private JMenuItem drawingPopupMenuNewVertex;
	private JMenuItem drawingPopupMenuDeleteVertex;
	private JCheckBox drawingPopupMenuConnectVertices;
	private JMenuItem drawingPopupMenuTileMode;
	private JMenuItem drawingPopupMenuCancel;
	
	private JPopupMenu tilingPopupMenu;
	private JMenuItem tilingPopupMenuBringSpriteToFront;
	private JMenuItem tilingPopupMenuSendSpriteToBack;
	private JMenuItem tilingPopupMenuDeleteSprite;
	private JMenuItem tilingPopupMenuDrawMode;
	private JMenuItem tilingPopupMenuCancel;
	
	private Point selectedGridBlock;
	public int mode;
	public boolean gridEnabled;
	final public static int MODE_TILING = 0;
	final public static int MODE_DRAWING = 1;
	final private static int DEFAULT_SELECTION_RADIUS = 6;
	private Point mousePosition;
	
	public static Color DEFAULT_GRID_COLOUR = new Color(64, 64, 64);
	public static Color DEFAULT_LINE_COLOUR = new Color(0, 0, 0);
	public static Color DEFAULT_VERTEX_COLOUR = new Color(0, 0, 0);
	public static Color DEFAULT_SELECTED_COLOUR = new Color(255, 0, 0);
	public static Color DEFAULT_BACKGROUND_COLOUR = new Color(173, 244, 255);
	
	public Color selectedColour;
	public Color gridColour;
	public Color lineColour;
	public Color vertexColour;
	public Color backgroundColour;
	
	final private int doubleClickSpeed = 200;
	private long lastMouseDown = 0;
	
	public EditorPanel(EditorWindow editorWindow, Variables settings) {
		world = null;
		setLayout(null);
		addMouseListener(this);
		addMouseMotionListener(this);
		
		this.editorWindow = editorWindow;
		if(editorWindow == null) {
			System.out.println("ERROR: Editor Window cannot be null.");
			System.exit(1);
		}
		
		createPopupMenus();
		
		mode = MODE_TILING;
		gridEnabled = true;
		selectedGridBlock = null;
		
		selectedPoint = null;
		selectedVertex = null;
		vertexToMove = null;
		lastSelectedVertex = null;
		selectedSprite = null;
		spriteToMove = null;
	
		loadSettings(settings);
		
		this.update();
	}
	
	private void loadSettings(Variables settings) {
		gridColour = DEFAULT_GRID_COLOUR;
		lineColour = DEFAULT_LINE_COLOUR;
		vertexColour = DEFAULT_VERTEX_COLOUR;
		selectedColour = DEFAULT_SELECTED_COLOUR;
		backgroundColour = DEFAULT_BACKGROUND_COLOUR;
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
	
	private void createPopupMenus() {
		drawingPopupMenu = new JPopupMenu();
		tilingPopupMenu = new JPopupMenu();
		
		drawingPopupMenuNewVertex = new JMenuItem("Create Vertex");
		drawingPopupMenuDeleteVertex = new JMenuItem("Delete Vertex");
		drawingPopupMenuConnectVertices = new JCheckBox("Auto-Connect Points");
		drawingPopupMenuTileMode = new JMenuItem("Texture Tiling Mode");
		drawingPopupMenuCancel = new JMenuItem("Cancel");
		
		tilingPopupMenuBringSpriteToFront = new JMenuItem("Bring Sprite to Front");
		tilingPopupMenuSendSpriteToBack = new JMenuItem("Send Sprite to Back");
		tilingPopupMenuDeleteSprite = new JMenuItem("Delete Sprite");
		tilingPopupMenuDrawMode = new JMenuItem("Boundary Drawing Mode");
		tilingPopupMenuCancel = new JMenuItem("Cancel");
		
		drawingPopupMenuConnectVertices.setSelected(true);
		
		drawingPopupMenuNewVertex.addActionListener(this);
		drawingPopupMenuDeleteVertex.addActionListener(this);
		drawingPopupMenuConnectVertices.addActionListener(this);
		drawingPopupMenuTileMode.addActionListener(this);
		
		tilingPopupMenuBringSpriteToFront.addActionListener(this);
		tilingPopupMenuSendSpriteToBack.addActionListener(this);
		tilingPopupMenuDeleteSprite.addActionListener(this);
		tilingPopupMenuDrawMode.addActionListener(this);
		
		drawingPopupMenu.add(drawingPopupMenuNewVertex);
		drawingPopupMenu.add(drawingPopupMenuDeleteVertex);
		drawingPopupMenu.add(drawingPopupMenuConnectVertices);
		drawingPopupMenu.addSeparator();
		drawingPopupMenu.add(drawingPopupMenuTileMode);
		drawingPopupMenu.add(drawingPopupMenuCancel);
		
		tilingPopupMenu.add(tilingPopupMenuBringSpriteToFront);
		tilingPopupMenu.add(tilingPopupMenuSendSpriteToBack);
		tilingPopupMenu.add(tilingPopupMenuDeleteSprite);
		tilingPopupMenu.addSeparator();
		tilingPopupMenu.add(tilingPopupMenuDrawMode);
		tilingPopupMenu.add(tilingPopupMenuCancel);
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
		if(e.getButton() == MouseEvent.BUTTON1) {
			if(mode == MODE_DRAWING && e.getWhen() - lastMouseDown < doubleClickSpeed) {
				Vertex newVertex = new Vertex(e.getPoint());
				world.addVertex(newVertex);
				if(drawingPopupMenuConnectVertices.isSelected() && lastSelectedVertex != null) {
					world.addEdge(new Edge(lastSelectedVertex, newVertex));
				}
			}
		}
		else if(e.getButton() == MouseEvent.BUTTON2) {
			if(mode == MODE_DRAWING) {
				vertexToMove = null;
				Vertex previousVertex = selectedVertex;
				
				selectedPoint = e.getPoint();
				selectVertex(e.getPoint(), DEFAULT_SELECTION_RADIUS);
				vertexToMove = selectedVertex;
				
				selectedVertex = previousVertex;
				lastSelectedVertex = selectedVertex;
			}
			else if(mode == MODE_TILING) {
				editorWindow.activeSprite = null;
				vertexToMove = null;
				
				selectSprite(e.getPoint());
				spriteToMove = selectedSprite;
			}
		}
		
		lastMouseDown = e.getWhen();
		this.update();
	}
	
	public void mouseReleased(MouseEvent e) {
		if(e.getButton() == MouseEvent.BUTTON3) {
			if(mode == MODE_DRAWING) {
				selectedPoint = e.getPoint();
				selectVertex(e.getPoint(), DEFAULT_SELECTION_RADIUS);
				drawingPopupMenuDeleteVertex.setEnabled(selectedVertex != null);
				drawingPopupMenu.show(this, e.getX(), e.getY());
			}
			else if(mode == MODE_TILING) {
				selectedPoint = e.getPoint();
				selectSprite(e.getPoint());
				tilingPopupMenuBringSpriteToFront.setEnabled(selectedSprite != null);
				tilingPopupMenuSendSpriteToBack.setEnabled(selectedSprite != null);
				tilingPopupMenuDeleteSprite.setEnabled(selectedSprite != null);
				tilingPopupMenu.show(this, e.getX(), e.getY());
			}
		}
		else if(e.getButton() == MouseEvent.BUTTON2) {
			vertexToMove = null;
			spriteToMove = null;
		}
		else if(e.getButton() == MouseEvent.BUTTON1) {
			if(mode == MODE_DRAWING) {
				Vertex previousVertex = null;
				if(selectedVertex != null) {
					previousVertex = selectedVertex; 
				}
				selectedPoint = e.getPoint();
				selectVertex(e.getPoint(), DEFAULT_SELECTION_RADIUS);
				
				if(previousVertex != null && selectedVertex != null && !previousVertex.equals(selectedVertex)) {
					Edge newEdge = new Edge(previousVertex, selectedVertex);
					
					if(this.world.containsEdge(newEdge) ||
					   this.world.containsEdge(new Edge(selectedVertex, previousVertex))) {
						return;
					}
					
					int result = JOptionPane.showConfirmDialog(this, "Create edge?", "Edge Creation", JOptionPane.YES_NO_OPTION);
					if(result == JOptionPane.YES_OPTION) {
						this.world.addEdge(newEdge);
					}
				}
			}
			else if(mode == MODE_TILING) {
				if(selectedGridBlock != null && editorWindow.activeSprite != null) {
					Vertex v = new Vertex((editorWindow.activeSprite.isTiled()) ? selectedGridBlock.x : (int) (e.getX() - (editorWindow.activeSprite.getWidth() / 2.0f)),
							 			  (editorWindow.activeSprite.isTiled()) ? selectedGridBlock.y : (int) (e.getY() - (editorWindow.activeSprite.getHeight() / 2.0f)));
					Entity newEntity = new Entity(v, editorWindow.activeSprite);
					newEntity.spriteSheetIndex = editorWindow.spriteSheets.getSpriteSheetIndex(editorWindow.activeSprite.getParentName());
					if(newEntity.getSprite().getType() == Sprite.TYPE_PLAYER) {
						world.setPlayer(newEntity);
					}
					else if(newEntity.getType() == Sprite.TYPE_PET) {
						world.setPet(newEntity);
					}
					else if(newEntity.getType() == Sprite.TYPE_AI) {
						world.addAI(newEntity);
					}
					else if(newEntity.getType() == Sprite.TYPE_ITEM) {
						world.addItem(newEntity);
					}
					else {
						if(newEntity.isTiled()) {
							world.addTile(newEntity);
						}
						else {
							world.addObject(newEntity);
						}
					}
				}
			}
		}
		this.update();
	}
	
	public void mouseDragged(MouseEvent e) {
		mousePosition = e.getPoint();
		if(mode == MODE_TILING) {
			getSelectedGridBlock(e.getPoint());
		}
		if(mode == MODE_DRAWING) {
			if(vertexToMove != null) {
				vertexToMove.x = e.getX();
				vertexToMove.y = e.getY();
			}
		}
		else if(mode == MODE_TILING) {
			if(spriteToMove != null) {
				spriteToMove.location.x = spriteToMove.isTiled() ? selectedGridBlock.x : (int) (e.getX() - (spriteToMove.getWidth() / 2.0f));
				spriteToMove.location.y = spriteToMove.isTiled() ? selectedGridBlock.y : (int) (e.getY() - (spriteToMove.getHeight() / 2.0f));
			}
		}
		this.update();
	}
	
	public void mouseMoved(MouseEvent e) {
		mousePosition = e.getPoint();
		if(mode == MODE_TILING) {
			getSelectedGridBlock(e.getPoint());
		}
		this.update();
	}
	
	public void actionPerformed(ActionEvent e) {
		if(world == null) { return; }
		
		if(mode == MODE_DRAWING) {
			if(e.getSource() == drawingPopupMenuNewVertex) {
				Vertex newVertex = new Vertex(selectedPoint.x, selectedPoint.y);
				world.addVertex(newVertex);
				lastSelectedVertex = null;
				selectedVertex = null;
			}
			else if(e.getSource() == drawingPopupMenuDeleteVertex) {
				world.removeVertex(selectedVertex);
				selectedVertex = null;
			}
			else if(e.getSource() == drawingPopupMenuTileMode) {
				mode = MODE_TILING;
			}
		}
		else if(mode == MODE_TILING) {
			if(e.getSource() == tilingPopupMenuBringSpriteToFront) {
				world.bringSpriteToFront(selectedSprite);
			}
			else if(e.getSource() == tilingPopupMenuSendSpriteToBack) {
				world.sendSpriteToBack(selectedSprite);
			}
			else if(e.getSource() == tilingPopupMenuDeleteSprite) {
				world.deleteSprite(selectedSprite);
			}
			else if(e.getSource() == tilingPopupMenuDrawMode) {
				mode = MODE_DRAWING;
			}
		}
		this.update();
	}
	
	public void selectVertex(Point p, int r) {
		if(p == null) { return; }
		if(r < 0) { r = 6; }
		selectedVertex = null;
		if(world != null) {
			for(int i=0;i<world.numberOfVertices();i++) {
				Vertex v = world.getVertex(i);
				if(Math.sqrt( Math.pow( (selectedPoint.x - v.x) , 2) + Math.pow( (selectedPoint.y - v.y) , 2) ) <= r) {
					selectedVertex = v;
					lastSelectedVertex = selectedVertex;
				}
			}
		}
	}
	
	public void selectSprite(Point p) {
		if(p == null) { return; }
		selectedSprite = null;
		if(world != null) {
			if(world.hasPet()) {
				if(p.x >= world.getPet().location.x &&
				   p.y >= world.getPet().location.y &&
				   p.x <= world.getPet().location.x + world.getPet().getWidth() &&
				   p.y <= world.getPet().location.y + world.getPet().getHeight()) {
					selectedSprite = world.getPet();
				}
			}
			if(selectedSprite == null && world.hasPlayer()) {
				if(p.x >= world.getPlayer().location.x &&
				   p.y >= world.getPlayer().location.y &&
				   p.x <= world.getPlayer().location.x + world.getPlayer().getWidth() &&
				   p.y <= world.getPlayer().location.y + world.getPlayer().getHeight()) {
					selectedSprite = world.getPlayer();
				}
			}
			if(selectedSprite == null) {
				for(int i=world.numberOfItems()-1;i>=0;i--) {
					Entity e = world.getItem(i);
					if(p.x >= e.location.x &&
					   p.y >= e.location.y &&
					   p.x <= e.location.x + e.getWidth() &&
					   p.y <= e.location.y + e.getHeight()) {
						selectedSprite = e;
						break;
					}
				}
			}
			if(selectedSprite == null) {
				for(int i=world.numberOfAI()-1;i>=0;i--) {
					Entity e = world.getAI(i);
					if(p.x >= e.location.x &&
					   p.y >= e.location.y &&
					   p.x <= e.location.x + e.getWidth() &&
					   p.y <= e.location.y + e.getHeight()) {
						selectedSprite = e;
						break;
					}
				}
			}
			if(selectedSprite == null) {
				for(int i=world.numberOfObjects()-1;i>=0;i--) {
					Entity e = world.getObject(i);
					if(p.x >= e.location.x &&
					   p.y >= e.location.y &&
					   p.x <= e.location.x + e.getWidth() &&
					   p.y <= e.location.y + e.getHeight()) {
						selectedSprite = e;
						break;
					}
				}
			}
			if(selectedSprite == null) {
				for(int i=world.numberOfTiles()-1;i>=0;i--) {
					Entity e = world.getTile(i);
					if(selectedGridBlock.x >= e.location.x &&
					   selectedGridBlock.y >= e.location.y &&
					   selectedGridBlock.x <= e.location.x + (e.getWidth() / World.GRID_SIZE) - 1 &&
					   selectedGridBlock.y <= e.location.y + (e.getHeight() / World.GRID_SIZE) - 1) {
						selectedSprite = e;
						break;
					}
				}
			}
		}
	}
	
	public void getSelectedGridBlock(Point p) {
		if(world == null) { return; }
		
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
		
		g.clearRect(0, 0, this.getWidth(), this.getHeight());
		
		g.setColor(backgroundColour);
		g.fillRect(0, 0, this.getWidth(), this.getHeight());
		
		drawObjects(g);
		
		drawGrid(g);
		
		if(mode == MODE_TILING && selectedGridBlock != null && editorWindow.activeSprite != null) {
			int xPos = editorWindow.activeSprite.isTiled() ? selectedGridBlock.x * World.GRID_SIZE : (mousePosition == null) ? 0 : (int) (mousePosition.x - (editorWindow.activeSprite.getWidth() / 2.0f));
			int yPos = editorWindow.activeSprite.isTiled() ? selectedGridBlock.y * World.GRID_SIZE : (mousePosition == null) ? 0 : (int) (mousePosition.y - (editorWindow.activeSprite.getHeight() / 2.0f));
			editorWindow.activeSprite.paintOn(g, xPos, yPos);
		}
		
		if(world != null) {
			world.paintOn(g, lineColour, vertexColour);
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
					Sprite sprite = null;
					int x = 0, y = 0;
					if(editorWindow.activeSprite != null && editorWindow.activeSprite.isTiled()) {
						sprite = editorWindow.activeSprite;
						x = selectedGridBlock.x;
						y = selectedGridBlock.y;
					}
					else if(spriteToMove != null && spriteToMove.isTiled()) {
						sprite = spriteToMove.getSprite();
						x = spriteToMove.location.x;
						y = spriteToMove.location.y;
					}
					if(sprite != null) {
						Graphics2D g2 = (Graphics2D) g;
						Stroke s = g2.getStroke();
						g2.setStroke(new BasicStroke(2));
						g2.setColor(selectedColour);
						int d = World.GRID_SIZE;
						int w = sprite.getWidth();
						int h = sprite.getHeight();
						g2.drawLine( x*d,     y*d,   (x*d)+w,  y*d);
						g2.drawLine((x*d)+w,  y*d,   (x*d)+w, (y*d)+h);
						g2.drawLine((x*d)+w, (y*d)+h, x*d,    (y*d)+h);
						g2.drawLine( x*d,    (y*d)+h, x*d,     y*d);
						g2.setStroke(s);
					}
				}
			}
			else {
				g.drawLine(0, 0, 0, world.gridSize.y * World.GRID_SIZE);
				g.drawLine(world.gridSize.x * World.GRID_SIZE, 0, world.gridSize.x * World.GRID_SIZE, world.gridSize.y * World.GRID_SIZE);
				g.drawLine(0, 0, world.gridSize.x * World.GRID_SIZE, 0);
				g.drawLine(0, world.gridSize.y * World.GRID_SIZE, world.gridSize.x * World.GRID_SIZE, world.gridSize.y * World.GRID_SIZE);
			}
		}
	}
	
	public void drawObjects(Graphics g) {
		if(world == null) { return; }
		
		for(int i=0;i<world.numberOfTiles();i++) {
			world.getTile(i).paintOn(g);
		}
		
		for(int i=0;i<world.numberOfObjects();i++) {
			world.getObject(i).paintOn(g);
		}
		
		for(int i=0;i<world.numberOfAI();i++) {
			world.getAI(i).paintOn(g);
		}
		
		if(world.hasPlayer()) {
			world.getPlayer().paintOn(g);
		}
		
		if(world.hasPet()) {
			world.getPet().paintOn(g);
		}

		for(int i=0;i<world.numberOfItems();i++) {
			world.getItem(i).paintOn(g);
		}
	}
	
	public void reset() {
		selectedVertex = null;
		vertexToMove = null;
		lastSelectedVertex = null;
		
		selectedSprite = null;
		spriteToMove = null;
	}
	
	public void update() {
		this.repaint();
	}
}
