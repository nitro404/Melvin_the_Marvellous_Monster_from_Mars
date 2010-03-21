import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class EditorPanel extends JPanel implements Scrollable, ActionListener, MouseListener, MouseMotionListener {
	
	private static final long serialVersionUID = 1L;
	
	private World world;
	
	private Vertex selectedPoint;
	private Vertex selectedVertex;
	
	private JPopupMenu popupMenu;
	private JMenuItem popupMenuNewVertex;
	private JMenuItem popupMenuDeleteVertex;
	private JMenuItem popupMenuCancel;
	
	public Sprite activeSprite;
	public static Sprite ALIEN_SPRITESHEET_IMAGE;
	public static SpriteSheet ALIEN_SPRITESHEET;
	public static Sprite ALIEN;
	
	private Point selectedGridBlock;
	public int mode;
	public boolean gridEnabled;
	final public static int MODE_TILING = 0;
	final public static int MODE_DRAWING = 1;
	
	public EditorPanel() {
		world = null;
		setLayout(null);
		addMouseListener(this);
		addMouseMotionListener(this);
		
		createPopupMenu();
		
		mode = MODE_TILING;
		gridEnabled = true;
		selectedGridBlock = null;
		activeSprite = null;
		
		loadImages();
		
		update();
	}
	
	public void createPopupMenu() {
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
			ALIEN_SPRITESHEET_IMAGE = new Sprite("Alien.png", EditorWindow.SPRITE_DIRECTORY);
			ALIEN_SPRITESHEET = new SpriteSheet(ALIEN_SPRITESHEET_IMAGE, 1, 1, 126, 126, 128, 128, true, 8, 8);
			ALIEN = ALIEN_SPRITESHEET.getSprite(5);
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
	public void mousePressed(MouseEvent e) { }
	
	public void mouseReleased(MouseEvent e) {
		if(mode == MODE_DRAWING) {
			if(e.getButton() == MouseEvent.BUTTON3) {
				selectedPoint = new Vertex(e.getPoint());
				selectVertex();
				popupMenuDeleteVertex.setEnabled(selectedVertex != null);
				popupMenu.show(this, e.getX(), e.getY());
			}
			else if(e.getButton() == MouseEvent.BUTTON1) {
				Vertex previousVertex = null;
				if(selectedVertex != null) {
					previousVertex = selectedVertex; 
				}
				selectedPoint = new Vertex(e.getPoint());
				selectVertex();
				
				if(previousVertex != null && selectedVertex != null && !previousVertex.equals(selectedVertex)) {
					int result = JOptionPane.showConfirmDialog(this, "Create edge?", "Edge Creation", JOptionPane.YES_NO_OPTION);
					if(result == JOptionPane.YES_OPTION) {
						this.world.addEdge(new Edge(new Vertex(previousVertex.x, previousVertex.y),
													new Vertex(selectedVertex.x, selectedVertex.y)));
					}
				}
			}
		}
		else if(mode == MODE_TILING) {
			if(selectedGridBlock != null && activeSprite == ALIEN) {
				world.objects.add(new Entity(new Vertex(selectedGridBlock.x, selectedGridBlock.y), 0, 0));
			}
		}
		this.update();
	}
	public void mouseDragged(MouseEvent e) {
		/*
		if(mode == MODE_DRAWING) {
			if(selectedVertex != null) {
				selectedVertex.x = e.getPoint().x;
				selectedVertex.y = e.getPoint().y;
			}
		}
		this.repaint();
		*/
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
	
	public void selectVertex() {
		double maxRadius = 6;
		selectedVertex = null;
		if(world != null) {
			for(int i=0;i<world.edges.verticies.size();i++) {
				Vertex v = world.edges.verticies.elementAt(i);
				if(Math.sqrt( Math.pow( (selectedPoint.x - v.x) , 2) + Math.pow( (selectedPoint.y - v.y) , 2) ) <= maxRadius) {
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
			world.paintOn(g);
		}
		
		drawObjects(g);
		
		drawGrid(g);
		
		if(mode == MODE_TILING && selectedGridBlock != null && activeSprite != null) {
			activeSprite.paintOn(g, selectedGridBlock.x * World.GRID_SIZE, selectedGridBlock.y * World.GRID_SIZE);
		}
	}
	
	public void drawGrid(Graphics g) {
		if(world != null) {
			g.setColor(new Color(64, 64, 64));
			
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
					g2.setColor(new Color(255, 0, 0));
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
		
		g.setColor(Color.RED);
		if(selectedVertex != null && mode == MODE_DRAWING) {
			selectedVertex.paintOn(g);
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
