import java.util.Vector;
import java.awt.*;
import java.io.*;

public class World {
	
	public Vertex gridSize;
	public Dimension dimensions;
	Graph edges;
	public Vector<Entity> mines;
	public Vector<Entity> rocks;
	public Vector<Entity> trees;
	public Vector<Entity> fences;
	public Vector<Entity> sheep;
	Vector<String> textureNames;
	
	final public static String WORLD_TYPE = "2D World";
	final public static double WORLD_VERSION = 1.2;
	
	final public static int GRID_WIDTH = 64;
	final public static int GRID_HEIGHT = 64;
	
	final public static int CARTESIAN_GRID_INCREMENT = 48;
	
//	public static int HORIZONTAL_OFFSET;
	
	final public static int ISOMETRIC_GRID_WIDTH = World.getIsometricWidth(GRID_WIDTH, GRID_HEIGHT);
	final public static int ISOMETRIC_GRID_HEIGHT = World.getIsometricHeight(GRID_WIDTH, GRID_HEIGHT);
	final public static double ISOMETRIC_HORIZONTAL_DOWNSCALE = 45.0 / 32.0;
	final public static double ISOMETRIC_VERTICAL_DOWNSCALE = 45.0 / 64.0;
	final public static double ISOMETRIC_HORIZONTAL_UPSCALE = 32.0 / 45.0;
	final public static double ISOMETRIC_VERTICAL_UPSCALE = 64.0 / 45.0;
	final public static double ISOMETRIC_GRID_ANGLE_RAD = Math.PI / 4.0;
	final public static double ISOMETRIC_GRID_ANGLE_DEG = ISOMETRIC_GRID_ANGLE_RAD * (180.0 / Math.PI);
	
	public World() {
		this.edges = new Graph();
		this.mines = new Vector<Entity>();
		this.rocks = new Vector<Entity>();
		this.trees = new Vector<Entity>();
		this.fences = new Vector<Entity>();
		this.sheep = new Vector<Entity>();
		this.textureNames = new Vector<String>();
	}
	
	public void addEdge(Edge e) {
		this.edges.addEdge(e);
	}

	public void addVertex(Vertex v) {
		this.edges.addVertex(v);
	}
	
	public void removeVertex(Vertex v) {
		edges.verticies.remove(v);
		Vector<Edge> edgesToRemove = new Vector<Edge>();
		for(int i=0;i<edges.size();i++) {
			if(this.edges.edges.elementAt(i).a.equals(v) || this.edges.edges.elementAt(i).b.equals(v)) {
				edgesToRemove.add(this.edges.edges.elementAt(i));
			}
		}
		for(int i=0;i<edgesToRemove.size();i++) {
			this.edges.edges.remove(edgesToRemove.elementAt(i));
		}
	}
	
	public void removeEdge(Edge e) {
		edges.edges.remove(e);
	}
	
	public static int getIsometricWidth(int width, int height) {
		return (int) Math.sqrt(Math.pow(width, 2) + Math.pow(height, 2));
	}
	
	public static int getIsometricHeight(int width, int height) {
		return (int) (getIsometricWidth(width, height) / 2);
	}
	
	public static int getIsometricX(int x, int y) { // rotate, then scale
		double xPos = x, yPos = y;
		double constCos = Math.cos(ISOMETRIC_GRID_ANGLE_RAD);
		double constSin = Math.sin(ISOMETRIC_GRID_ANGLE_RAD);
		xPos = (constCos * xPos) + ((-constSin) * yPos);
		xPos *= ISOMETRIC_HORIZONTAL_DOWNSCALE;
		return (int) xPos;
	}
	
	public static int getIsometricY(int x, int y) { // rotate, then scale
		double xPos = x, yPos = y;
		double constCos = Math.cos(ISOMETRIC_GRID_ANGLE_RAD);
		double constSin = Math.sin(ISOMETRIC_GRID_ANGLE_RAD);
		yPos = (constSin * xPos) + (constCos * yPos);
		yPos *= ISOMETRIC_VERTICAL_DOWNSCALE;
		return (int) yPos;
	}
	
	public static Point getIsometricPoint(Point p) {
		return new Point(getIsometricX(p.x, p.y), getIsometricY(p.x, p.y));
	}
	
	public static int getCartesianX(int x, int y) { // scale, then rotate
		double xPos = x, yPos = y;
		double constCos = Math.cos(ISOMETRIC_GRID_ANGLE_RAD);
		double constSin = Math.sin(ISOMETRIC_GRID_ANGLE_RAD);
		xPos *= ISOMETRIC_HORIZONTAL_UPSCALE;
		yPos *= ISOMETRIC_VERTICAL_UPSCALE;
		double tempX = xPos, tempY = yPos;
		xPos = (constCos * tempX) + (constSin * tempY);
		yPos = ((-constSin) * tempX) + (constCos * tempY);
		return (int) xPos;
	}
	
	public static int getCartesianY(int x, int y) { // scale, then rotate
		double xPos = x, yPos = y;
		double constCos = Math.cos(ISOMETRIC_GRID_ANGLE_RAD);
		double constSin = Math.sin(ISOMETRIC_GRID_ANGLE_RAD);
		xPos *= ISOMETRIC_HORIZONTAL_UPSCALE;
		yPos *= ISOMETRIC_VERTICAL_UPSCALE;
		double tempX = xPos, tempY = yPos;
		xPos = (constCos * tempX) + (constSin * tempY);
		yPos = ((-constSin) * tempX) + (constCos * tempY);
		return (int) yPos;
	}
	
	public static Point getCartesianPoint(Point p) {
		return new Point(getCartesianX(p.x, p.y), getCartesianY(p.x, p.y));
	}
	
	public static World parseFrom(String fileName) {
		if(fileName == null || fileName.trim().length() == 0) {
			return null;
		}
		try {
			BufferedReader in = new BufferedReader(new FileReader(fileName.trim()));
			World world = parseFrom(in);
			if(in != null) {
				in.close();
			}
			return world;
		}
		catch(FileNotFoundException e) {
			System.out.println("ERROR: Unable to open file: \"" + fileName + "\"");
			e.printStackTrace();
		}
		catch(IOException e) {
			System.out.println("ERROR: " + e.getMessage());
			e.printStackTrace();
		}
		catch(Exception e) {
			System.out.println("ERROR: Corrupted map file.");
			e.printStackTrace();
		}
		return null;
	}
	
	public static World parseFrom(BufferedReader in) throws IOException {
		if(in == null) {
			return null;
		}
		
		String input;
		World world = new World();
		
		// input the world header
		input = in.readLine();
		String worldType = input.substring(0, input.indexOf(':', 0)).trim();
		if(!WORLD_TYPE.equals(worldType)) {
			System.out.println("ERROR: Incompatible world type (" + worldType + "). Current editor only supports worlds of type " + WORLD_TYPE + ".");
			return null;
		}
		double worldVersion = Double.valueOf(input.substring(input.lastIndexOf(' ', input.length() - 1), input.length()).trim());
		if(WORLD_VERSION != worldVersion) {
			System.out.println("ERROR: Incompatible map version (" + worldVersion + "). Current editor only supports version " + WORLD_VERSION + ".");
			return null;
		}
		
		// read in the map dimensions
		input = in.readLine();
		String dimensionsHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!dimensionsHeader.equals("Dimensions")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Dimensions\", found \"" + dimensionsHeader + "\".");
			return null;
		}
		int mapWidth = Integer.valueOf(input.substring(input.indexOf(':', 0) + 1, input.indexOf(',', 0)).trim());
		int mapHeight = Integer.valueOf(input.substring(input.indexOf(',', 0) + 1, input.length()).trim());
		world.gridSize = new Vertex(mapWidth, mapHeight);
		//world.dimensions = new Dimension(mapWidth * ISOMETRIC_GRID_WIDTH, mapHeight * ISOMETRIC_GRID_HEIGHT);
		world.dimensions = new Dimension(mapWidth * (CARTESIAN_GRID_INCREMENT * 2), mapHeight * CARTESIAN_GRID_INCREMENT);
//		HORIZONTAL_OFFSET = (world.dimensions.width / 2) + 1;
		
		// read in the texture names
		input = in.readLine();
		String texturesHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!texturesHeader.equals("Textures")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Textures\", found \"" + texturesHeader + "\".");
			return null;
		}
		int numberOfTextures = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		for(int i=0;i<numberOfTextures;i++) {
			input = in.readLine().trim();
			if(input != null && input.length() != 0) {
				if(!world.textureNames.contains(input)) {
					world.textureNames.add(input);
				}
			}
		}
		
		// read in the corresponding edges for each barrier
		input = in.readLine();
		String edgesHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!edgesHeader.equals("Edges")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Edges\", found \"" + edgesHeader + "\".");
			return null;
		}
		int numberOfEdges = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		for(int j=0;j<numberOfEdges;j++) {
			input = in.readLine().trim();
			world.addEdge(Edge.parseFrom(input));
		}
		
		// read in the mines
		input = in.readLine();
		String minesHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!minesHeader.equals("Mines")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Mines\", found \"" + minesHeader + "\".");
			return null;
		}
		int numberOfMines = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		Entity newMine;
		for(int i=0;i<numberOfMines;i++) {
			input = in.readLine().trim();
			newMine = Entity.parseFrom(input);
			if(!world.mines.contains(newMine)) {
				world.mines.add(newMine);
			}
		}

		// read in the rocks
		input = in.readLine();
		String rocksHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!rocksHeader.equals("Rocks")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Rocks\", found \"" + rocksHeader + "\".");
			return null;
		}
		int numberOfRocks = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		Entity newRock;
		for(int i=0;i<numberOfRocks;i++) {
			input = in.readLine().trim();
			newRock = Entity.parseFrom(input);
			if(!world.rocks.contains(newRock)) {
				world.rocks.add(newRock);
			}
		}

		// read in the trees
		input = in.readLine();
		String treesHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!treesHeader.equals("Trees")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Trees\", found \"" + treesHeader + "\".");
			return null;
		}
		int numberOfTrees = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		Entity newTree;
		for(int i=0;i<numberOfTrees;i++) {
			input = in.readLine().trim();
			newTree = Entity.parseFrom(input);
			if(!world.trees.contains(newTree)) {
				world.trees.add(newTree);
			}
		}

		// read in the fences
		input = in.readLine();
		String fencesHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!fencesHeader.equals("Fences")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Fences\", found \"" + fencesHeader + "\".");
			return null;
		}
		int numberOfFences = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		Entity newFence;
		for(int i=0;i<numberOfFences;i++) {
			input = in.readLine().trim();
			newFence = Entity.parseFrom(input);
			if(!world.fences.contains(newFence)) {
				world.fences.add(newFence);
			}
		}

		// read in the sheep
		input = in.readLine();
		String sheepHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!sheepHeader.equals("Sheep")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Sheep\", found \"" + sheepHeader + "\".");
			return null;
		}
		int numberOfSheep = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		Entity newSheep;
		for(int i=0;i<numberOfSheep;i++) {
			input = in.readLine().trim();
			newSheep = Entity.parseFrom(input);
			if(!world.sheep.contains(newSheep)) {
				world.sheep.add(newSheep);
			}
		}
		
		return world;
	}
	
	public void writeTo(String fileName) {
		if(fileName == null || fileName.trim().length() == 0) {
			return;
		}
		try {
			PrintWriter out = new PrintWriter(new FileWriter(fileName));
			this.writeTo(out);
			if(out != null) {
				out.close();
			}
		}
		catch(FileNotFoundException e) {
			System.out.println("ERROR: Unable to open file: \"" + fileName + "\"");
			e.printStackTrace();
		}
		catch(IOException e) {
			System.out.println("ERROR: " + e.getMessage());
			e.printStackTrace();
		}
		catch(Exception e) {
			System.out.println("ERROR: Corrupted map file.");
			e.printStackTrace();
		}
	}
	
	public void writeTo(PrintWriter out) throws IOException {
		// write the world header and version
		out.println(WORLD_TYPE + ": Version " + WORLD_VERSION);
		
		// write the dimensions
		out.print("Dimensions: ");
		gridSize.writeTo(out);
		out.println();
		
		// write the textures
		out.println("Textures: " + this.textureNames.size());
		for(int i=0;i<this.textureNames.size();i++) {
			out.println("\t" + this.textureNames.elementAt(i));
		}
		
		// write the edges
		out.println("Edges: " + this.edges.size());
		this.edges.writeTo(out);
		
		// write the mines
		out.println("Mines: " + this.mines.size());
		for(int i=0;i<this.mines.size();i++) {
			out.print("\t");
			this.mines.elementAt(i).writeTo(out);
			out.println();
		}

		// write the rocks
		out.println("Rocks: " + this.rocks.size());
		for(int i=0;i<this.rocks.size();i++) {
			out.print("\t");
			this.rocks.elementAt(i).writeTo(out);
			out.println();
		}

		// write the trees
		out.println("Trees: " + this.trees.size());
		for(int i=0;i<this.trees.size();i++) {
			out.print("\t");
			this.trees.elementAt(i).writeTo(out);
			out.println();
		}

		// write the fences
		out.println("Fences: " + this.fences.size());
		for(int i=0;i<this.fences.size();i++) {
			out.print("\t");
			this.fences.elementAt(i).writeTo(out);
			out.println();
		}

		// write the sheep
		out.println("Sheep: " + this.sheep.size());
		for(int i=0;i<this.sheep.size();i++) {
			out.print("\t");
			this.sheep.elementAt(i).writeTo(out);
			out.println();
		}
	}

	public void paintOn(Graphics g) {
		edges.paintOn(g);
	}
	
	public boolean equals(Object o) {
		if(o == null || !(o instanceof World)) {
			return false;
		}
		
		World w = (World) o;
		
		if(this.mines.size() != w.mines.size()) {
			return false;
		}
		for(int i=0;i<this.mines.size();i++) {
			if(!w.mines.contains(this.mines.elementAt(i))) {
				return false;
			}
		}
		
		if(this.rocks.size() != w.rocks.size()) {
			return false;
		}
		for(int i=0;i<this.rocks.size();i++) {
			if(!w.rocks.contains(this.rocks.elementAt(i))) {
				return false;
			}
		}
		
		if(this.trees.size() != w.trees.size()) {
			return false;
		}
		for(int i=0;i<this.trees.size();i++) {
			if(!w.trees.contains(this.trees.elementAt(i))) {
				return false;
			}
		}

		if(this.fences.size() != w.fences.size()) {
			return false;
		}
		for(int i=0;i<this.fences.size();i++) {
			if(!w.fences.contains(this.fences.elementAt(i))) {
				return false;
			}
		}

		if(this.sheep.size() != w.sheep.size()) {
			return false;
		}
		for(int i=0;i<this.sheep.size();i++) {
			if(!w.sheep.contains(this.sheep.elementAt(i))) {
				return false;
			}
		}
		
		return this.edges.equals(w.edges);
	}
	
	public String toString() {
		return "World";
	}
	
}
