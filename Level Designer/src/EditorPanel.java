import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

public class EditorPanel extends JPanel implements Scrollable, ActionListener, MouseListener, MouseMotionListener {
	
	private static final long serialVersionUID = 1L;
	
	private World world;
	
	private EditorWindow editorWindow;
	
	private Vertex selectedPoint;
	private Vertex selectedVertex;
	private Vertex vertexToMove;
	private Vertex lastSelectedVertex;
	
	private JPopupMenu popupMenu;
	private JMenuItem popupMenuNewVertex;
	private JMenuItem popupMenuDeleteVertex;
	private JCheckBox popupMenuConnectVertices;
	private JMenuItem popupMenuTileMode;
	private JMenuItem popupMenuDrawMode;
	private JMenuItem popupMenuCancel;
	
	private Point selectedGridBlock;
	public int mode;
	public boolean gridEnabled;
	final public static int MODE_TILING = 0;
	final public static int MODE_DRAWING = 1;
	final private static int DEFAULT_SELECTION_RADIUS = 6;
	private boolean autoConnectVertices;
	private Point mousePosition;
	
	public static Color DEFAULT_GRID_COLOUR = new Color(64, 64, 64);
	public static Color DEFAULT_LINE_COLOUR = new Color(0, 0, 0);
	public static Color DEFAULT_VERTEX_COLOUR = new Color(0, 0, 0);
	public static Color DEFAULT_SELECTED_COLOUR = new Color(255, 0, 0);
	
	public Color selectedColour;
	public Color gridColour;
	public Color lineColour;
	public Color vertexColour; 
	
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
		
		createPopupMenu();
		
		mode = MODE_TILING;
		gridEnabled = true;
		selectedGridBlock = null;
		autoConnectVertices = false;
		
		selectedPoint = null;
		selectedVertex = null;
		vertexToMove = null;
		lastSelectedVertex = null;
	
		loadSettings(settings);
		
		update();
	}
	
	private void loadSettings(Variables settings) {
		gridColour = DEFAULT_GRID_COLOUR;
		lineColour = DEFAULT_LINE_COLOUR;
		vertexColour = DEFAULT_VERTEX_COLOUR;
		selectedColour = DEFAULT_SELECTED_COLOUR;
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
		popupMenuConnectVertices = new JCheckBox("Auto-Connect Points");
		popupMenuTileMode = new JMenuItem("Texture Tiling Mode");
		popupMenuDrawMode = new JMenuItem("Boundary Drawing Mode");
		popupMenuCancel = new JMenuItem("Cancel");
		
		popupMenuNewVertex.addActionListener(this);
		popupMenuDeleteVertex.addActionListener(this);
		popupMenuConnectVertices.addActionListener(this);
		popupMenuTileMode.addActionListener(this);
		popupMenuDrawMode.addActionListener(this);
		
		popupMenu.add(popupMenuNewVertex);
		popupMenu.add(popupMenuDeleteVertex);
		popupMenu.add(popupMenuConnectVertices);
		popupMenu.addSeparator();
		popupMenu.add(popupMenuTileMode);
		popupMenu.add(popupMenuDrawMode);
		popupMenu.addSeparator();
		popupMenu.add(popupMenuCancel);
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
				if(autoConnectVertices && lastSelectedVertex != null) {
					world.addEdge(new Edge(lastSelectedVertex, newVertex));
				}
			}
		}
		else if(mode == MODE_DRAWING && e.getButton() == MouseEvent.BUTTON2) {
			vertexToMove = null;
			Vertex previousVertex = selectedVertex;
			Vertex previousPoint = selectedPoint;
			
			selectedPoint = new Vertex(e.getPoint());
			selectVertex(e.getPoint(), DEFAULT_SELECTION_RADIUS);
			vertexToMove = selectedVertex; 
			
			selectedVertex = previousVertex;
			lastSelectedVertex = selectedVertex;
			previousVertex = previousPoint;
		}
		lastMouseDown = e.getWhen();
	}
	
	public void mouseReleased(MouseEvent e) {
		if(e.getButton() == MouseEvent.BUTTON3) {
			if(mode == MODE_DRAWING) {
				selectedPoint = new Vertex(e.getPoint());
				selectVertex(e.getPoint(), DEFAULT_SELECTION_RADIUS);
			}
			popupMenuDeleteVertex.setEnabled(selectedVertex != null && mode == MODE_DRAWING);
			popupMenu.show(this, e.getX(), e.getY());
		}
		if(mode == MODE_DRAWING) {
			if(e.getButton() == MouseEvent.BUTTON1) {
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
				if(selectedGridBlock != null && editorWindow.activeSprite != null) {
					Vertex v = new Vertex((editorWindow.activeSprite.isTiled()) ? selectedGridBlock.x : e.getPoint().x,
							 			  (editorWindow.activeSprite.isTiled()) ? selectedGridBlock.y : e.getPoint().y);
					Entity newEntity = new Entity(v, editorWindow.activeSprite);
					newEntity.spriteSheetIndex = editorWindow.spriteSheets.getSpriteSheetIndex(editorWindow.activeSprite.getParentName());
					if(newEntity.isTiled()) {
						world.tiles.add(newEntity);
					}
					else {
						world.objects.add(newEntity);
					}
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
		mousePosition = e.getPoint();
		if(mode == MODE_TILING) {
			getSelectedGridBlock(e.getPoint());
		}
		this.repaint();
	}
	
	public void actionPerformed(ActionEvent e) {
		if(world == null) { return; }
		if(mode == MODE_DRAWING) {
			if(e.getSource() == popupMenuNewVertex) {
				Vertex newVertex = new Vertex(selectedPoint.x, selectedPoint.y);
				world.addVertex(newVertex);
				if(autoConnectVertices && lastSelectedVertex != null) {
					world.addEdge(new Edge(lastSelectedVertex, newVertex));
				}
				selectedVertex = null;
			}
			else if(e.getSource() == popupMenuDeleteVertex) {
				world.removeVertex(selectedVertex);
				selectedVertex = null;
			}
		}
		if(e.getSource() == popupMenuTileMode) {
			mode = MODE_TILING;
		}
		else if(e.getSource() == popupMenuDrawMode) {
			mode = MODE_DRAWING;
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
					lastSelectedVertex = selectedVertex;
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
		
		if(mode == MODE_TILING && selectedGridBlock != null && editorWindow.activeSprite != null) {
			int xPos = editorWindow.activeSprite.isTiled() ? selectedGridBlock.x * World.GRID_SIZE : (mousePosition == null) ? 0 : mousePosition.x;
			int yPos = editorWindow.activeSprite.isTiled() ? selectedGridBlock.y * World.GRID_SIZE : (mousePosition == null) ? 0 : mousePosition.y;
			editorWindow.activeSprite.paintOn(g, xPos, yPos);
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
				
				if(mode == MODE_TILING && selectedGridBlock != null &&
				   editorWindow.activeSprite != null && editorWindow.activeSprite.isTiled()) {
					Graphics2D g2 = (Graphics2D) g;
					Stroke s = g2.getStroke();
					g2.setStroke(new BasicStroke(2));
					g2.setColor(selectedColour);
					int d = World.GRID_SIZE;
					int w = editorWindow.activeSprite.getWidth();
					int h = editorWindow.activeSprite.getHeight();
					int x = selectedGridBlock.x;
					int y = selectedGridBlock.y;
					g2.drawLine( x*d,     y*d,   (x*d)+w,  y*d);
					g2.drawLine((x*d)+w,  y*d,   (x*d)+w, (y*d)+h);
					g2.drawLine((x*d)+w, (y*d)+h, x*d,    (y*d)+h);
					g2.drawLine( x*d,    (y*d)+h, x*d,     y*d);
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
		Entity e;
		Sprite s;
		int index;
		
		for(int i=0;i<world.tiles.size();i++) {
			e = world.tiles.elementAt(i);
			index = e.spriteSheetIndex;
			if(index < 0) { editorWindow.spriteSheets.getSpriteSheetIndex(e.getSprite().getParentName()); }
			if(index >= 0) {
				s = editorWindow.spriteSheets.getSpriteSheet(index).getSprite(e.getSpriteIndex());
				if(s != null) {
					s.paintOn(g, e.location.x * World.GRID_SIZE, e.location.y * World.GRID_SIZE);
				}
			}
		}
		
		for(int i=0;i<world.objects.size();i++) {
			e = world.objects.elementAt(i);
			index = e.spriteSheetIndex;
			if(index < 0) { editorWindow.spriteSheets.getSpriteSheetIndex(e.getSprite().getParentName()); }
			if(index >= 0) {
				s = editorWindow.spriteSheets.getSpriteSheet(index).getSprite(e.getSpriteIndex());
				if(s != null) {
					s.paintOn(g, e.location.x, e.location.y);
				}
			}
		}
	}
	
	public void update() {
		this.autoConnectVertices = popupMenuConnectVertices.isSelected();
		
		popupMenuNewVertex.setEnabled(mode == MODE_DRAWING);
		popupMenuConnectVertices.setEnabled(mode == MODE_DRAWING);
		popupMenuTileMode.setEnabled(mode == MODE_DRAWING);
		popupMenuDrawMode.setEnabled(mode == MODE_TILING);
		
		this.repaint();
	}
}
