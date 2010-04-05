import java.util.Vector;
import java.awt.*;
import java.io.*;

public class World {
	
	public Vertex gridSize;
	public Dimension dimensions;
	private Graph graph;
	private Entity player;
	private Entity pet;
	private Vector<Entity> objects;
	private Vector<Entity> tiles;
	private Vector<Entity> ai;
	private Vector<Entity> items;
	
	final public static String WORLD_TYPE = "2D Cartesian World";
	final public static double WORLD_VERSION = 1.0;
	
	final public static int GRID_SIZE = 64;
	
	public World() {
		this.graph = new Graph();
		this.player = null;
		this.pet = null;
		this.objects = new Vector<Entity>();
		this.tiles = new Vector<Entity>();
		this.ai = new Vector<Entity>();
		this.items = new Vector<Entity>();
	}
	
	public void addEdge(Edge e) {
		this.graph.addEdge(e);
	}

	public void addVertex(Vertex v) {
		this.graph.addVertex(v);
	}
	
	public int numberOfVertices() {
		return this.graph.vertices.size();
	}
	
	public int numberOfEdges() {
		return this.graph.edges.size();
	}
	
	public boolean containsVertex(Vertex v) {
		return this.graph.containsVertex(v);
	}
	
	public boolean containsEdge(Edge e) {
		return this.graph.containsEdge(e);
	}

	public Vertex getVertex(int index) {
		if(index < 0 || index >= this.graph.vertices.size()) { return null; }
		
		return this.graph.vertices.elementAt(index);
	}

	public Edge getEdge(int index) {
		if(index < 0 || index >= this.graph.edges.size()) { return null; }
		
		return this.graph.edges.elementAt(index);
	}
	
	public boolean removeVertex(Vertex v) {
		boolean removed = graph.vertices.remove(v);
		Vector<Edge> edgesToRemove = new Vector<Edge>();
		for(int i=0;i<graph.size();i++) {
			if(this.graph.edges.elementAt(i).a.equals(v) || this.graph.edges.elementAt(i).b.equals(v)) {
				edgesToRemove.add(this.graph.edges.elementAt(i));
			}
		}
		for(int i=0;i<edgesToRemove.size();i++) {
			this.graph.edges.remove(edgesToRemove.elementAt(i));
		}
		return removed;
	}
	
	public boolean removeEdge(Edge e) {
		return graph.edges.remove(e);
	}
	
	public void addObject(Entity e) {
		if(e == null ||
		   e.getSprite() == null ||
		   e.isTiled() ||
		   e.getSprite().getType() == Sprite.TYPE_PLAYER ||
		   e.getSprite().getType() == Sprite.TYPE_PET ||
		   e.getSprite().getType() == Sprite.TYPE_AI ||
		   e.getSprite().getType() == Sprite.TYPE_ITEM) { return; }
		
		if(!this.objects.contains(e)) {
			this.objects.add(e);
		}
	}
	
	public void addTile(Entity e) {
		if(e == null ||
		   e.getSprite() == null ||
		   !e.isTiled() ||
		   e.getSprite().getType() == Sprite.TYPE_PLAYER ||
		   e.getSprite().getType() == Sprite.TYPE_PET ||
		   e.getSprite().getType() == Sprite.TYPE_AI) { return; }
		
		if(!this.tiles.contains(e)) {
			this.tiles.add(e);
		}
	}
	
	public void addAI(Entity e) {
		if(e == null || e.getSprite() == null || !(e.getSprite().getType() == Sprite.TYPE_AI)) { return; }
		
		if(!this.ai.contains(e)) {
			this.ai.add(e);
		}
	}
	
	public void addItem(Entity e) {
		if(e == null || e.getSprite() == null || !(e.getSprite().getType() == Sprite.TYPE_ITEM)) { return; }
		
		if(!this.items.contains(e)) {
			this.items.add(e);
		}
	}
	
	public int numberOfObjects() {
		return this.objects.size();
	}
	
	public int numberOfTiles() {
		return this.tiles.size();
	}
	
	public int numberOfAI() {
		return this.ai.size();
	}
	
	public int numberOfItems() {
		return this.items.size();
	}
	
	public boolean containsObject(Entity e) {
		if(e == null) { return false; }
		
		return this.objects.contains(e);
	}
	
	public boolean containsTile(Entity e) {
		if(e == null) { return false; }
		
		return this.tiles.contains(e);
	}
	
	public boolean containsAI(Entity e) {
		if(e == null) { return false; }
		
		return this.ai.contains(e);
	}
	
	public boolean containsItems(Entity e) {
		if(e == null) { return false; }
		
		return this.items.contains(e);
	}
	
	public boolean removeObject(Entity e) {
		if(e == null) { return false; }
		
		return this.objects.remove(e);
	}
	
	public boolean removeTile(Entity e) {
		if(e == null) { return false; }
		
		return this.tiles.remove(e);
	}
	
	public boolean removeAI(Entity e) {
		if(e == null) { return false; }
		
		return this.ai.remove(e);
	}
	
	public boolean removeItem(Entity e) {
		if(e == null) { return false; }
		
		return this.items.remove(e);
	}
	
	public boolean removeObject(int index) {
		if(index < 0 || index >= this.objects.size()) { return false; }
		
		return this.objects.remove(index) != null;
	}
	
	public boolean removeTile(int index) {
		if(index < 0 || index >= this.tiles.size()) { return false; }
		
		return this.tiles.remove(index) != null;
	}
	
	public boolean removeAI(int index) {
		if(index < 0 || index >= this.ai.size()) { return false; }
		
		return this.ai.remove(index) != null;
	}
	
	public boolean removeItem(int index) {
		if(index < 0 || index >= this.items.size()) { return false; }
		
		return this.items.remove(index) != null;
	}
	
	public Entity getObject(int index) {
		if(index < 0 || index >= this.objects.size()) { return null; }
		
		return this.objects.elementAt(index);
	}
	
	public Entity getTile(int index) {
		if(index < 0 || index >= this.tiles.size()) { return null; }
		
		return this.tiles.elementAt(index);
	}
	
	public Entity getAI(int index) {
		if(index < 0 || index >= this.ai.size()) { return null; }
		
		return this.ai.elementAt(index);
	}
	
	public Entity getItem(int index) {
		if(index < 0 || index >= this.items.size()) { return null; }
		
		return this.items.elementAt(index);
	}
	
	public void bringSpriteToFront(Entity e) {
		if(e == null) { return; }
		
		if(this.objects.remove(e)) {
			this.objects.add(e);
		}
		else if(this.tiles.remove(e)) {
			this.tiles.add(e);
		}
	}
	
	public void sendSpriteToBack(Entity e) {
		if(e == null) { return; }
		
		if(this.objects.remove(e)) {
			this.objects.insertElementAt(e, 0);
		}
		else if(this.tiles.remove(e)) {
			this.tiles.insertElementAt(e, 0);
		}
	}
	
	public void deleteSprite(Entity e) {
		this.removeObject(e);
		this.removeTile(e);
	}
	
	public Entity getPlayer() {
		return this.player;
	}
	
	public boolean hasPlayer() {
		return this.player != null;
	}
	
	public void setPlayer(Entity e) {
		if(e == null || e.getSprite() == null || !(e.getSprite().getType() == Sprite.TYPE_PLAYER)) { return; }
		
		this.player = e;
	}
	
	public Entity getPet() {
		return this.pet;
	}
	
	public boolean hasPet() {
		return this.pet != null;
	}
	
	public void setPet(Entity e) {
		if(e == null || e.getSprite() == null || !(e.getSprite().getType() == Sprite.TYPE_PET)) { return; }
		
		this.pet = e;
	}
	
	public static World parseFrom(String fileName, SpriteSheets spriteSheets) {
		if(fileName == null || fileName.trim().length() == 0) {
			return null;
		}
		try {
			BufferedReader in = new BufferedReader(new FileReader(fileName.trim()));
			World world = readFrom(in, spriteSheets);
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
	
	public static World readFrom(BufferedReader in, SpriteSheets spriteSheets) throws IOException {
		if(in == null) {
			return null;
		}
		
		String input;
		World world = new World();
		
		// input the world header
		input = in.readLine();
		String worldType = input.substring(0, input.indexOf(':', 0)).trim();
		if(!WORLD_TYPE.equalsIgnoreCase(worldType)) {
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
		if(!gridSizeHeader.equalsIgnoreCase("Grid Size")) {
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
		if(!dimensionsHeader.equalsIgnoreCase("Dimensions")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Dimensions\", found \"" + dimensionsHeader + "\".");
			return null;
		}
		int mapWidth = Integer.valueOf(input.substring(input.indexOf(':', 0) + 1, input.indexOf(',', 0)).trim());
		int mapHeight = Integer.valueOf(input.substring(input.indexOf(',', 0) + 1, input.length()).trim());
		world.gridSize = new Vertex(mapWidth, mapHeight);
		world.dimensions = new Dimension(mapWidth * GRID_SIZE, mapHeight * GRID_SIZE);
		
		// read in the corresponding edges for each barrier
		input = in.readLine();
		String edgesHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!edgesHeader.equalsIgnoreCase("Edges")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Edges\", found \"" + edgesHeader + "\".");
			return null;
		}
		int numberOfEdges = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		for(int j=0;j<numberOfEdges;j++) {
			input = in.readLine().trim();
			world.addEdge(Edge.parseFrom(input));
		}

		// read in the player spawn position
		input = in.readLine();
		String playerHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!playerHeader.equalsIgnoreCase("Player")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Player\", found \"" + playerHeader + "\".");
			return null;
		}
		world.setPlayer(Entity.parseFrom(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim(), spriteSheets));
		
		// read in the pet spawn position
		input = in.readLine();
		String petHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!petHeader.equalsIgnoreCase("Pet")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Pet\", found \"" + petHeader + "\".");
			return null;
		}
		String petData = input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim();
		if(!petData.equalsIgnoreCase("None")) {
			world.setPet(Entity.parseFrom(petData, spriteSheets));
		}
		
		// read in the objects
		input = in.readLine();
		String objectsHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!objectsHeader.equalsIgnoreCase("Objects")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Objects\", found \"" + objectsHeader + "\".");
			return null;
		}
		int numberOfObjects = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		for(int i=0;i<numberOfObjects;i++) {
			input = in.readLine().trim();
			world.addObject(Entity.parseFrom(input, spriteSheets));
		}
		
		// read in the tiles
		input = in.readLine();
		String tilesHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!tilesHeader.equalsIgnoreCase("Tiles")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Tiles\", found \"" + tilesHeader + "\".");
			return null;
		}
		int numberOfTiles = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		for(int i=0;i<numberOfTiles;i++) {
			input = in.readLine().trim();
			world.addTile(Entity.parseFrom(input, spriteSheets));
		}
		
		// read in the ai
		input = in.readLine();
		String aiHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!aiHeader.equalsIgnoreCase("AI")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"AI\", found \"" + aiHeader + "\".");
			return null;
		}
		int numberOfAI = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		for(int i=0;i<numberOfAI;i++) {
			input = in.readLine().trim();
			world.addAI(Entity.parseFrom(input, spriteSheets));
		}
		
		// read in the items
		input = in.readLine();
		String itemsHeader = input.substring(0, input.indexOf(':', 0)).trim();
		if(!itemsHeader.equalsIgnoreCase("Items")) {
			System.out.println("ERROR: Corrupted world file. Expected header \"Items\", found \"" + itemsHeader + "\".");
			return null;
		}
		int numberOfItems = Integer.valueOf(input.substring(input.lastIndexOf(':', input.length() - 1) + 1, input.length()).trim());
		for(int i=0;i<numberOfItems;i++) {
			input = in.readLine().trim();
			world.addItem(Entity.parseFrom(input, spriteSheets));
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
		
		// write the edges
		out.println("Edges: " + this.graph.size());
		this.graph.writeTo(out);
		
		// write the player position
		out.print("Player: ");
		this.player.writeTo(out);
		out.println();
		
		// write the pet position
		out.print("Pet: ");
		if(this.hasPet()) {
			this.pet.writeTo(out);
			out.println();
		}
		else {
			out.println("None");
		}
		
		// write the objects
		out.println("Objects: " + this.objects.size());
		for(int i=0;i<this.objects.size();i++) {
			out.print("\t");
			this.objects.elementAt(i).writeTo(out);
			out.println();
		}
		
		// write the tiles
		out.println("Tiles: " + this.tiles.size());
		for(int i=0;i<this.tiles.size();i++) {
			out.print("\t");
			this.tiles.elementAt(i).writeTo(out);
			out.println();
		}
		
		// write the ai
		out.println("AI: " + this.ai.size());
		for(int i=0;i<this.ai.size();i++) {
			out.print("\t");
			this.ai.elementAt(i).writeTo(out);
			out.println();
		}
		
		// write the items
		out.println("Items: " + this.items.size());
		for(int i=0;i<this.items.size();i++) {
			out.print("\t");
			this.items.elementAt(i).writeTo(out);
			out.println();
		}
	}

	public void paintOn(Graphics g, Color lineColour, Color vertexColour) {
		graph.paintOn(g, lineColour, vertexColour);
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
		
		if(this.tiles.size() != w.tiles.size()) {
			return false;
		}
		for(int i=0;i<this.tiles.size();i++) {
			if(!w.tiles.contains(this.tiles.elementAt(i))) {
				return false;
			}
		}
		
		if(this.ai.size() != w.ai.size()) {
			return false;
		}
		for(int i=0;i<this.ai.size();i++) {
			if(!w.ai.contains(this.ai.elementAt(i))) {
				return false;
			}
		}
		
		if(this.items.size() != w.items.size()) {
			return false;
		}
		for(int i=0;i<this.items.size();i++) {
			if(!w.items.contains(this.items.elementAt(i))) {
				return false;
			}
		}
		
		if(this.player == null && w.player != null ||
				   this.player != null && w.player == null) {
					return false;
				}
				if(this.player != null && w.player != null &&
				   !(this.player.equals(w.player))) {
					return false;
				}
		
		if(this.pet == null && w.pet != null ||
		   this.pet != null && w.pet == null) {
			return false;
		}
		if(this.pet != null && w.pet != null &&
		   !(this.pet.equals(w.pet))) {
			return false;
		}
		
		return this.graph.equals(w.graph);
	}
	
	public String toString() {
		return "World";
	}
	
}
