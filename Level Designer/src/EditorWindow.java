import java.io.*;
import java.util.StringTokenizer;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.event.*;
import javax.swing.*;

public class EditorWindow extends JFrame implements ActionListener {
	
	private static final long serialVersionUID = 1L;
	
	final public static String DEFAULT_SETTINGS_FILE = "settings.ini";
	final public static String DEFAULT_MAP_DIRECTORY = "..\\Maps";
	final public static String DEFAULT_SPRITE_DIRECTORY = "..\\Sprites";
	private int DEFAULT_EDITOR_WIDTH = 1024;
	private int DEFAULT_EDITOR_HEIGHT = 768;
	private int DEFAULT_XPOS = 0;
	private int DEFAULT_YPOS = 0;
	
	private String settingsFileName;
	public Variables settings;
	public String mapDirectory;
	public String spriteDirectory;
	private Dimension initialEditorSize;
	private Point initialEditorPosition;
	
	public World world;
	
	final public static int DEFAULT_WIDTH = 15;
	final public static int DEFAULT_HEIGHT = 10;
	
	private JMenuBar menu;
	private JMenu menuFile;
	private JMenuItem menuFileNewMap;
	private JMenuItem menuFileOpenMap;
	private JMenuItem menuFileSaveMap;
	private JMenuItem menuFileExit;
	private JMenu menuView;
	private JMenuItem menuViewPalette;
	private JMenuItem menuViewDimensions;
	private JMenuItem menuViewGridToggle;
	private JMenu menuMode;
	private JMenuItem menuModeTile;
	private JMenuItem menuModeDraw;
	private JMenu menuHelp;
	private JMenuItem menuHelpAbout;
	
	public EditorPanel editorPanel;
	private JScrollPane editorPanelScrollPane;
	
	private PaletteWindow paletteWindow;
	
	private JFileChooser fileChooser; 
	
	public EditorWindow(String settingsFileName) {
		super("2D Cartesian Level Editor");
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		
		if(!loadSettings(settingsFileName)) {
			mapDirectory = DEFAULT_MAP_DIRECTORY;
			spriteDirectory = DEFAULT_SPRITE_DIRECTORY;
			initialEditorPosition = new Point(DEFAULT_XPOS, DEFAULT_YPOS);
			initialEditorSize = new Dimension(DEFAULT_EDITOR_WIDTH, DEFAULT_EDITOR_HEIGHT);
		}
		
		setSize(initialEditorSize.width, initialEditorSize.height);
		setLocation(initialEditorPosition.x, initialEditorPosition.y);
		
		fileChooser = new JFileChooser();
		fileChooser.setCurrentDirectory(new File(System.getProperty("user.dir") + "\\" + mapDirectory).getAbsoluteFile());
		
		createMenu();
		
		editorPanel = new EditorPanel(spriteDirectory, settings);
		editorPanelScrollPane = new JScrollPane(editorPanel);
		add(editorPanelScrollPane);
		
		paletteWindow = new PaletteWindow(this, settings);
		paletteWindow.setVisible(true);
		
		createNewMap();
		
		update();
	}
	
	private boolean loadSettings(String settingsFileName) {
		this.settingsFileName = settingsFileName;
		if(settingsFileName == null || settingsFileName.trim().length() == 0) {
			this.settingsFileName = DEFAULT_SETTINGS_FILE;
		}
		
		settings = new Variables();
		if(!settings.parseFrom(settingsFileName)) {
			settings = null;
			return false;
		}
		
		mapDirectory = settings.getValue("Map Directory");
		if(mapDirectory == null) {
			mapDirectory = DEFAULT_MAP_DIRECTORY;
		}
		spriteDirectory = settings.getValue("Sprite Directory");
		if(spriteDirectory != null) {
			File temp = new File(spriteDirectory);
			if(!temp.exists() || !temp.isDirectory()) {
				spriteDirectory = DEFAULT_SPRITE_DIRECTORY;
			}
		}
		else {
			spriteDirectory = DEFAULT_SPRITE_DIRECTORY;
		}
		
		initialEditorPosition = Utilities.parsePoint(settings.getValue("Editor Window Position"));
		if(initialEditorPosition == null) { initialEditorPosition = new Point(DEFAULT_XPOS, DEFAULT_YPOS); }
		
		initialEditorSize = Utilities.parseDimension(settings.getValue("Editor Window Size"));
		if(initialEditorSize == null) { initialEditorSize = new Dimension(DEFAULT_EDITOR_WIDTH, DEFAULT_EDITOR_HEIGHT); }
		
		return true;
	}
	
	//gridColor = JColorChooser.showDialog(this, "Choose Grid Colour", gridColor);
	private void saveSettings() {
		PrintWriter out;
		try {
			out = new PrintWriter(new FileWriter(settingsFileName));
			out.println("Map Directory: " + mapDirectory);
			out.println("Sprite Directory: " + spriteDirectory);
			out.println("Editor Window Position: " + this.getLocation().x + ", " + this.getLocation().y);
			out.println("Editor Window Size: " + this.getWidth() + ", " + this.getHeight());
			out.println("Palette Window Position: " + this.paletteWindow.getLocation().x + ", " + this.paletteWindow.getLocation().y);
			out.println("Palette Window Size: " + this.paletteWindow.getWidth() + ", " + this.paletteWindow.getHeight());
			out.println("Grid Colour: " + this.editorPanel.gridColour.getRed() + ", " + this.editorPanel.gridColour.getGreen() + ", " + this.editorPanel.gridColour.getBlue());
			out.println("Line Colour: " + this.editorPanel.lineColour.getRed() + ", " + this.editorPanel.lineColour.getGreen() + ", " + this.editorPanel.lineColour.getBlue());
			out.println("Vertex Colour: " + this.editorPanel.vertexColour.getRed() + ", " + this.editorPanel.vertexColour.getGreen() + ", " + this.editorPanel.vertexColour.getBlue());
			out.println("Selected Colour: " + this.editorPanel.selectedColour.getRed() + ", " + this.editorPanel.selectedColour.getGreen() + ", " + this.editorPanel.selectedColour.getBlue());
			out.close();
		}
		catch(IOException e) { }
	}
	
	private void createMenu() {
		menu = new JMenuBar();
		
		menuFile = new JMenu("File");
		menuFileNewMap = new JMenuItem("New Map");
		menuFileOpenMap = new JMenuItem("Open Map");
		menuFileSaveMap = new JMenuItem("Save Map");
		menuFileExit = new JMenuItem("Exit");
		menuView = new JMenu("View");
		menuViewPalette = new JMenuItem("Palette");
		menuViewDimensions = new JMenuItem("Map Dimensions");
		menuViewGridToggle = new JMenuItem("Toggle Grid");
		menuMode = new JMenu("Mode");
		menuModeTile = new JMenuItem("Tile Textures");
		menuModeDraw = new JMenuItem("Draw Boundaries");
		menuHelp = new JMenu("Help");
		menuHelpAbout = new JMenuItem("About");
			
		menuFileNewMap.addActionListener(this);
		menuFileOpenMap.addActionListener(this);
		menuFileSaveMap.addActionListener(this);
		menuFileExit.addActionListener(this);
		menuViewPalette.addActionListener(this);
		menuViewDimensions.addActionListener(this);
		menuViewGridToggle.addActionListener(this);
		menuMode.addActionListener(this);
		menuModeTile.addActionListener(this);
		menuModeDraw.addActionListener(this);
		menuHelpAbout.addActionListener(this);
		
		menuFile.add(menuFileNewMap);
		menuFile.add(menuFileOpenMap);
		menuFile.add(menuFileSaveMap);
		menuFile.addSeparator();
		menuFile.add(menuFileExit);
		menuView.add(menuViewPalette);
		menuView.add(menuViewDimensions);
		menuView.add(menuViewGridToggle);
		menuMode.add(menuModeTile);
		menuMode.add(menuModeDraw);
		menuHelp.add(menuHelpAbout);
		
		menu.add(menuFile);
		menu.add(menuView);
		menu.add(menuMode);
		menu.add(menuHelp);
		
		setJMenuBar(menu);
	}
	
	public void actionPerformed(ActionEvent e) {
		if(e.getSource().equals(menuFileNewMap)) {
			createNewMap();
		}
		else if(e.getSource() == menuFileOpenMap) {
			if(fileChooser.showDialog(this, "Open Map") == JFileChooser.APPROVE_OPTION) {
				world = World.parseFrom(fileChooser.getSelectedFile().getAbsolutePath());
				editorPanel.setWorld(world);
			}
		}
		else if(e.getSource() == menuFileSaveMap) {
			if(fileChooser.showDialog(this, "Save Map") == JFileChooser.APPROVE_OPTION) {
				world.writeTo(fileChooser.getSelectedFile().getAbsolutePath());
			}
		}
		else if(e.getSource() == menuFileExit) {
			System.exit(0);
		}
		else if(e.getSource() == menuViewPalette) {
			paletteWindow.setVisible(!paletteWindow.isVisible());
		}
		else if(e.getSource() == menuViewDimensions) {
			setMapDimensions(world);
		}
		else if(e.getSource() == menuViewGridToggle) {
			editorPanel.gridEnabled = !editorPanel.gridEnabled;
			update();
		}
		else if(e.getSource() == menuModeTile) {
			editorPanel.mode = EditorPanel.MODE_TILING;
		}
		else if(e.getSource() == menuModeDraw) {
			editorPanel.mode = EditorPanel.MODE_DRAWING;
		}
		else if(e.getSource() == menuHelpAbout) {
			JOptionPane.showMessageDialog(this, "2D Level Designer created by Kevin Scroggins.", "2D Level Designer", JOptionPane.INFORMATION_MESSAGE);
		}
		
		update();
	}
	
	public void createNewMap() {
		this.world = new World();
		editorPanel.setWorld(world);
		setMapDimensions(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	}
	
	public void setMapDimensions(World world) {
		String data = JOptionPane.showInputDialog(this, "Please enter new map dimensions (ie. 8x8).", "Map Dimensions", JOptionPane.QUESTION_MESSAGE);
		if(data != null) {
			StringTokenizer st = new StringTokenizer(data.trim(), ",x");
			int newWidth = 0, newHeight = 0;
			boolean valid = true;
			if(st.countTokens() != 2) { valid = false; }
			if(valid) {
				try {
					newWidth = Integer.parseInt(st.nextToken().trim());
					newHeight = Integer.parseInt(st.nextToken().trim());
				}
				catch(NumberFormatException e) {
					valid = false;
				}
				if(newWidth <= 0 || newHeight <= 0) {
					valid = false;
				}
			}
			
			if(valid) {
				setMapDimensions(newWidth, newHeight);
			}
			else {
				JOptionPane.showMessageDialog(this, "Invalid map dimension entered, please the form \"AxB\" or \"A, B\".", "Invalid Dimension", JOptionPane.ERROR_MESSAGE);
			}
		}
		update();
	}
	
	public void setMapDimensions(int newWidth, int newHeight) {
		world.gridSize = new Vertex(newWidth, newHeight);
		world.dimensions = new Dimension((newWidth * World.GRID_SIZE) + 1, (newHeight * World.GRID_SIZE) + 1);
	}
	
	public void update() {
		this.repaint();
		editorPanelScrollPane.repaint();
		editorPanel.update();
	}
	
}
