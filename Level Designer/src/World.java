import java.util.Vector;
import java.awt.*;
import java.io.*;

public class World {
	
	public Vertex gridSize;
	public Dimension dimensions;
	public Graph edges;
	public Vector<Entity> objects;
	public Vector<String> textureNames;
	
	final public static String WORLD_TYPE = "2D Cartesian World";
	final public static double WORLD_VERSION = 1.0;
	
	final public static int GRID_SIZE = 64;
	
	public World() {
		this.edges = new Graph();
		this.objects = new Vector<Entity>();
		this.textureNames = new Vector<String>();
	}
	
	public void addEdge(Edge e) {
		this.edges.addEdge(e);
	}

	public void addVertex(Vertex v) {
		this.edges.addVertex(v);
	}
	
	public void removeVertex(Vertex v) {
		edges.vertices.remove(v);
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
		
		// read in the grid size
		input = in.readLine();
		String gridSizeHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!gridSizeHeader.equals("Grid Size")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Grid Size\", found \"" + gridSizeHeader + "\".");
			return null;
		}
		int fileGridSize = Integer.valueOf(input.substring(input.indexOf(':', 0) + 1, input.length()).trim());
		if(fileGridSize != GRID_SIZE) {
			System.out.println("ERROR: Incompatible grid size: " + fileGridSize + ". The editor only supports a grid size of " + GRID_SIZE + ".");
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
		world.dimensions = new Dimension(mapWidth * GRID_SIZE, mapHeight * GRID_SIZE);
		
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
		
		// read in the objects
		input = in.readLine();
		String objectsHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!objectsHeader.equals("Objects")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Objects\", found \"" + objectsHeader + "\".");
			return null;
		}
		int numberOfObjects = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		Entity newObject;
		for(int i=0;i<numberOfObjects;i++) {
			input = in.readLine().trim();
			newObject = Entity.parseFrom(input);
			if(!world.objects.contains(newObject)) {
				world.objects.add(newObject);
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
		
		// write grid size
		out.println("Grid Size: " + GRID_SIZE);
		
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
		
		// write the objects
		out.println("Objects: " + this.objects.size());
		for(int i=0;i<this.objects.size();i++) {
			out.print("\t");
			this.objects.elementAt(i).writeTo(out);
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
		
		if(this.objects.size() != w.objects.size()) {
			return false;
		}
		for(int i=0;i<this.objects.size();i++) {
			if(!w.objects.contains(this.objects.elementAt(i))) {
				return false;
			}
		}
		
		return this.edges.equals(w.edges);
	}
	
	public String toString() {
		return "World";
	}
	
}
