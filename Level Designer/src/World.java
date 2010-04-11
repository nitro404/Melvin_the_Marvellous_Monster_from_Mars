// ======================================= //
// Melvin the Marvellous Monster from Mars //
// Level Designer                          //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

import java.util.Vector;
import java.awt.*;
import java.io.*;

/**
 * The World class represents a Level in which the player can explore and interact with.
 * 
 * @author Kevin Scroggins
 */
public class World {
	
	/** The size of the Grid in the current World. */
	public Vertex gridSize;
	/** The dimensions of the current World. */
	public Dimension dimensions;
	/** The collection dynamic collision boundaries associated with the World. */
	private Graph graph;
	/** The instance of the Player. */
	private Entity player;
	/** The instance of the Player's Pet. */
	private Entity pet;
	/** The collection of static Objects located in the World. */
	private Vector<Entity> objects;
	/** The collection of tiles (grid-aligned Sprites) located in the World. */
	private Vector<Entity> tiles;
	/** The ai characters located in the World. */
	private Vector<Entity> ai;
	/** The items located within the World which the Player can interact with. */
	private Vector<Entity> items;
	
	/** A String representing the type of World that the Level Designer currently supports. */
	final public static String WORLD_TYPE = "2D Cartesian World";
	/** The version of the World that the Level Designer currently supports. */
	final public static double WORLD_VERSION = 1.0;
	/** The size of the Grid the Level Designer supports by default. */
	final public static int GRID_SIZE = 64;
	
	/**
	 * Constructs an empty World object.
	 */
	public World() {
		// initialise the collections associated with the World
		this.graph = new Graph();
		this.player = null;
		this.pet = null;
		this.objects = new Vector<Entity>();
		this.tiles = new Vector<Entity>();
		this.ai = new Vector<Entity>();
		this.items = new Vector<Entity>();
	}

	/**
	 * Adds a specified Vertex to the World's collision boundary collection.
	 * 
	 * @param v the Vertex to be added.
	 */
	public void addVertex(Vertex v) {
		this.graph.addVertex(v);
	}
	
	/**
	 * Adds a specified Edge to the World's collision boundary collection.
	 * 
	 * @param e the Edge to be added.
	 */
	public void addEdge(Edge e) {
		this.graph.addEdge(e);
	}
	
	/**
	 * Returns the number of Vertices contained within the World's collision boundary collection.
	 * 
	 * @return the number of Vertices contained within the World's collision boundary collection.
	 */
	public int numberOfVertices() {
		return this.graph.vertices.size();
	}
	
	/**
	 * Returns the number of Edges contained within the World's collision boundary collection.
	 * 
	 * @return the number of Edges contained within the World's collision boundary collection.
	 */
	public int numberOfEdges() {
		return this.graph.edges.size();
	}
	
	/**
	 * Checks if the specified Vertex is contained within the World's collision boundary collection.
	 * 
	 * @param v the Vertex to match.
	 * @return true if the specified Vertex exists.
	 */
	public boolean containsVertex(Vertex v) {
		return this.graph.containsVertex(v);
	}
	
	/**
	 * Checks if the specified Edge is contained within the World's collision boundary collection.
	 * 
	 * @param e the Edge to match.
	 * @return true if the specified Edge exists.
	 */
	public boolean containsEdge(Edge e) {
		return this.graph.containsEdge(e);
	}

	/**
	 * Returns the Vertex located at the specified index if the index is not out of bounds and there is at least one Vertex in the World's boundary collision collection.
	 * 
	 * @param index the index of the Vertex to return.
	 * @return the Vertex located at the specified index if the index is not out of bounds and there is at least one Vertex in the World's boundary collision collection.
	 */
	public Vertex getVertex(int index) {
		if(index < 0 || index >= this.graph.vertices.size()) { return null; }
		
		// return the Vertex located at the specified index
		return this.graph.vertices.elementAt(index);
	}

	/**
	 * Returns the Edge located at the specified index if the index is not out of bounds and there is at least one Edge in the World's boundary collision collection.
	 * 
	 * @param index the index of the Edge to return.
	 * @return the Edge located at the specified index if the index is not out of bounds and there is at least one Edge in the World's boundary collision collection.
	 */
	public Edge getEdge(int index) {
		if(index < 0 || index >= this.graph.edges.size()) { return null; }
		
		// return the Edge located at the specified index
		return this.graph.edges.elementAt(index);
	}
	
	/**
	 * Removes the specified Vertex from the World's boundary collision collection.
	 * 
	 * @param v the Vertex to remove.
	 * @return true if the specified Vertex was successfully removed.
	 */
	public boolean removeVertex(Vertex v) {
		// remove the specified Vertex
		boolean removed = graph.vertices.remove(v);
		
		// collect all Edges connected to the specified vertex
		Vector<Edge> edgesToRemove = new Vector<Edge>();
		for(int i=0;i<graph.size();i++) {
			if(this.graph.edges.elementAt(i).a.equals(v) || this.graph.edges.elementAt(i).b.equals(v)) {
				edgesToRemove.add(this.graph.edges.elementAt(i));
			}
		}
		
		// remove all collected Edges
		for(int i=0;i<edgesToRemove.size();i++) {
			this.graph.edges.remove(edgesToRemove.elementAt(i));
		}
		return removed;
	}
	
	/**
	 * Returns the specified Edge from the World's boundary collision collection.
	 * 
	 * @param e the Edge to be removed.
	 * @return true if the specified Edge was successfully removed.
	 */
	public boolean removeEdge(Edge e) {
		return graph.edges.remove(e);
	}
	
	/**
	 * Adds an Object to the World's collection of Objects if it does not already exist and is valid.
	 * 
	 * @param e the Object to be added.
	 */
	public void addObject(Entity e) {
		// verify that the Object is valid
		if(e == null ||
		   e.getSprite() == null ||
		   e.isTiled() ||
		   e.getSprite().getType() == Sprite.TYPE_PLAYER ||
		   e.getSprite().getType() == Sprite.TYPE_PET ||
		   e.getSprite().getType() == Sprite.TYPE_AI ||
		   e.getSprite().getType() == Sprite.TYPE_ITEM) { return; }
		
		// add the Object if it is not a duplicate
		if(!this.objects.contains(e)) {
			this.objects.add(e);
		}
	}
	
	/**
	 * Adds an Tile to the World's collection of Tiles if it does not already exist and is valid.
	 * 
	 * @param e the Tile to be added.
	 */
	public void addTile(Entity e) {
		// verify that the Tile is valid
		if(e == null ||
		   e.getSprite() == null ||
		   !e.isTiled() ||
		   e.getSprite().getType() == Sprite.TYPE_PLAYER ||
		   e.getSprite().getType() == Sprite.TYPE_PET ||
		   e.getSprite().getType() == Sprite.TYPE_AI) { return; }
		
		// add the Tile if it is not a duplicate
		if(!this.tiles.contains(e)) {
			this.tiles.add(e);
		}
	}
	
	/**
	 * Adds an AI to the World's collection of AI if it does not already exist and is valid.
	 * 
	 * @param e the AI to be added.
	 */
	public void addAI(Entity e) {
		// verify that the AI is valid
		if(e == null || e.getSprite() == null || !(e.getSprite().getType() == Sprite.TYPE_AI)) { return; }
		
		// add the AI if it is not a duplicate
		if(!this.ai.contains(e)) {
			this.ai.add(e);
		}
	}
	
	/**
	 * Adds an Item to the World's collection of Items if it does not already exist and is valid.
	 * 
	 * @param e the Item to be added.
	 */
	public void addItem(Entity e) {
		// verify that the Item is valid
		if(e == null || e.getSprite() == null || !(e.getSprite().getType() == Sprite.TYPE_ITEM)) { return; }
		
		// add the Item if it is not a duplicate
		if(!this.items.contains(e)) {
			this.items.add(e);
		}
	}
	
	/**
	 * Returns the number of Objects contained within the World.
	 * 
	 * @return the number of Objects contained within the World.
	 */
	public int numberOfObjects() {
		return this.objects.size();
	}
	
	/**
	 * Returns the number of Tiles contained within the World.
	 * 
	 * @return the number of Tiles contained within the World.
	 */
	public int numberOfTiles() {
		return this.tiles.size();
	}
	
	/**
	 * Returns the number of AI contained within the World.
	 * 
	 * @return the number of AI contained within the World.
	 */
	public int numberOfAI() {
		return this.ai.size();
	}
	
	/**
	 * Returns the number of Items contained within the World.
	 * 
	 * @return the number of Items contained within the World.
	 */
	public int numberOfItems() {
		return this.items.size();
	}
	
	/**
	 * Checks if the Object is contained within the World's collection of Objects.
	 * 
	 * @param e the Object to be matched.
	 * @return true if the Object exists.
	 */
	public boolean containsObject(Entity e) {
		if(e == null) { return false; }
		
		return this.objects.contains(e);
	}
	
	/**
	 * Checks if the Tile is contained within the World's collection of Tiles.
	 * 
	 * @param e the Tile to be matched.
	 * @return true if the Tile exists.
	 */
	public boolean containsTile(Entity e) {
		if(e == null) { return false; }
		
		return this.tiles.contains(e);
	}
	
	/**
	 * Checks if the AI is contained within the World's collection of AI.
	 * 
	 * @param e the AI to be matched.
	 * @return true if the AI exists.
	 */
	public boolean containsAI(Entity e) {
		if(e == null) { return false; }
		
		return this.ai.contains(e);
	}
	
	/**
	 * Checks if the Item is contained within the World's collection of Items.
	 * 
	 * @param e the Item to be matched.
	 * @return true if the Item exists.
	 */
	public boolean containsItems(Entity e) {
		if(e == null) { return false; }
		
		return this.items.contains(e);
	}
	
	/**
	 * Removes the specified Object from the World's collection of Objects.
	 * 
	 * @param e the Object to be removed.
	 * @return true of the Object was successfully removed.
	 */
	public boolean removeObject(Entity e) {
		if(e == null) { return false; }
		
		return this.objects.remove(e);
	}
	
	/**
	 * Removes the specified Tile from the World's collection of Tiles.
	 * 
	 * @param e the Tile to be removed.
	 * @return true of the Tile was successfully removed.
	 */
	public boolean removeTile(Entity e) {
		if(e == null) { return false; }
		
		return this.tiles.remove(e);
	}
	
	/**
	 * Removes the specified AI from the World's collection of AI.
	 * 
	 * @param e the AI to be removed.
	 * @return true of the AI was successfully removed.
	 */
	public boolean removeAI(Entity e) {
		if(e == null) { return false; }
		
		return this.ai.remove(e);
	}
	
	/**
	 * Removes the specified Item from the World's collection of Items.
	 * 
	 * @param e the Item to be removed.
	 * @return true of the Item was successfully removed.
	 */
	public boolean removeItem(Entity e) {
		if(e == null) { return false; }
		
		return this.items.remove(e);
	}
	
	/**
	 * Removes the Object at the specified index from the World's collection of Objects at the specified index.
	 * 
	 * @param index the index to remove an Object from.
	 * @return true if an Object was successfully removed at the specified index.
	 */
	public boolean removeObject(int index) {
		if(index < 0 || index >= this.objects.size()) { return false; }
		
		return this.objects.remove(index) != null;
	}
	
	/**
	 * Removes the Tile at the specified index from the World's collection of Tiles.
	 * 
	 * @param index the index to remove an Tile from.
	 * @return true if an Tile was successfully removed at the specified index.
	 */
	public boolean removeTile(int index) {
		if(index < 0 || index >= this.tiles.size()) { return false; }
		
		return this.tiles.remove(index) != null;
	}
	
	/**
	 * Removes the AI at the specified index from the World's collection of AI.
	 * 
	 * @param index the index to remove an AI from.
	 * @return true if an AI was successfully removed at the specified index.
	 */
	public boolean removeAI(int index) {
		if(index < 0 || index >= this.ai.size()) { return false; }
		
		return this.ai.remove(index) != null;
	}
	
	/**
	 * Removes the Item at the specified index from the World's collection of Items.
	 * 
	 * @param index the index to remove an Item from.
	 * @return true if an Item was successfully removed at the specified index.
	 */
	public boolean removeItem(int index) {
		if(index < 0 || index >= this.items.size()) { return false; }
		
		return this.items.remove(index) != null;
	}
	
	/**
	 * Returns the Object at the specified index if the index is within range and the World contains at least one Object, otherwise returns null.
	 * 
	 * @param index the index of the Object to return;
	 * @return the Object at the specified index if the index is within range and the World contains at least one Object, otherwise returns null.
	 */
	public Entity getObject(int index) {
		if(index < 0 || index >= this.objects.size()) { return null; }
		
		return this.objects.elementAt(index);
	}
	
	/**
	 * Returns the Tile at the specified index if the index is within range and the World contains at least one Tile, otherwise returns null.
	 * 
	 * @param index the index of the Tile to return;
	 * @return the Tile at the specified index if the index is within range and the World contains at least one Tile, otherwise returns null.
	 */
	public Entity getTile(int index) {
		if(index < 0 || index >= this.tiles.size()) { return null; }
		
		return this.tiles.elementAt(index);
	}
	
	/**
	 * Returns the AI at the specified index if the index is within range and the World contains at least one AI, otherwise returns null.
	 * 
	 * @param index the index of the AI to return;
	 * @return the AI at the specified index if the index is within range and the World contains at least one AI, otherwise returns null.
	 */
	public Entity getAI(int index) {
		if(index < 0 || index >= this.ai.size()) { return null; }
		
		return this.ai.elementAt(index);
	}
	
	/**
	 * Returns the Item at the specified index if the index is within range and the World contains at least one Item, otherwise returns null.
	 * 
	 * @param index the index of the Item to return;
	 * @return the Item at the specified index if the index is within range and the World contains at least one Item, otherwise returns null.
	 */
	public Entity getItem(int index) {
		if(index < 0 || index >= this.items.size()) { return null; }
		
		return this.items.elementAt(index);
	}
	
	/**
	 * Locates the current Entity within the World (if it exists) and brings it to the front of the screen.
	 * 
	 * @param e the Entity to be swapped.
	 */
	public void bringSpriteToFront(Entity e) {
		if(e == null) { return; }
		
		if(this.objects.remove(e)) {
			this.objects.add(e);
		}
		else if(this.tiles.remove(e)) {
			this.tiles.add(e);
		}
		else if(this.ai.remove(e)) {
			this.ai.add(e);
		}
		else if(this.items.remove(e)) {
			this.items.add(e);
		}
	}
	
	/**
	 * Locates the current Entity within the World (if it exists) and sends it to the back of the screen.
	 * 
	 * @param e the Entity to be swapped.
	 */
	public void sendSpriteToBack(Entity e) {
		if(e == null) { return; }
		
		if(this.objects.remove(e)) {
			this.objects.insertElementAt(e, 0);
		}
		else if(this.tiles.remove(e)) {
			this.tiles.insertElementAt(e, 0);
		}
		else if(this.ai.remove(e)) {
			this.ai.insertElementAt(e, 0);
		}
		else if(this.items.remove(e)) {
			this.items.insertElementAt(e, 0);
		}
	}
	
	/**
	 * Removes a specified Entity from the World.
	 * 
	 * @param e the entity to be removed.
	 */
	public void deleteSprite(Entity e) {
		this.removeObject(e);
		this.removeTile(e);
	}
	
	/**
	 * Returns the instance of the Player.
	 * 
	 * @return the instance of the Player.
	 */
	public Entity getPlayer() {
		return this.player;
	}
	
	/**
	 * Returns true if a Player spawn has been placed in the World.
	 * 
	 * @return true if a Player spawn has been placed in the World.
	 */
	public boolean hasPlayer() {
		return this.player != null;
	}
	
	/**
	 * Sets the Player instance.
	 * 
	 * @param e the Entity to instantiate the Player as.
	 */
	public void setPlayer(Entity e) {
		if(e == null || e.getSprite() == null || !(e.getSprite().getType() == Sprite.TYPE_PLAYER)) { return; }
		
		this.player = e;
	}
	
	/**
	 * Returns the instance of the Pet.
	 * 
	 * @return the instance of the Pet.
	 */
	public Entity getPet() {
		return this.pet;
	}
	
	/**
	 * Returns true if a Pet spawn has been placed in the World.
	 * 
	 * @return true if a Pet spawn has been placed in the World.
	 */
	public boolean hasPet() {
		return this.pet != null;
	}
	
	/**
	 * Sets the Pet instance.
	 * 
	 * @param e the Entity to instantiate the Pet as.
	 */
	public void setPet(Entity e) {
		if(e == null || e.getSprite() == null || !(e.getSprite().getType() == Sprite.TYPE_PET)) { return; }
		
		this.pet = e;
	}
	
	/**
	 * Parses a World from the specified file and instantiates all of the Entities using their corresponding Sprites found in the collection of SpriteSheets.
	 * 
	 * @param fileName the World file to load.
	 * @param spriteSheets the SpriteSheet collection to instantiate the Entity Sprites with.
	 * @return the World parsed from the specified input parameters.
	 */
	public static World parseFrom(String fileName, SpriteSheets spriteSheets) {
		if(fileName == null || fileName.trim().length() == 0) {
			return null;
		}
		try {
			// open the file and read the World from the input stream
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
	
	/**
	 * Parses a World from the specified input stream and instantiates all of the Entities using their corresponding Sprites found in the collection of SpriteSheets.
	 * 
	 * @param in the input stream to parse the World from.
	 * @param spriteSheets the SpriteSheet collection to instantiate the Entity Sprites with.
	 * @return the World parsed from the specified input parameters.
	 * @throws IOException if there was an error reading the World from the specified input stream.
	 */
	public static World readFrom(BufferedReader in, SpriteSheets spriteSheets) throws IOException {
		if(in == null) {
			return null;
		}
		
		String input;
		World world = new World();
		
		// input the world header and verify the type and version of the map
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
		
		// read in the grid size and verify it
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
		world.dimensions = new Dimension((mapWidth * GRID_SIZE) + 1, (mapHeight * GRID_SIZE) + 1);
		
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
	
	/**
	 * Outputs the current world to the specified file.
	 * 
	 * @param fileName the file to output the World to.
	 */
	public void writeTo(String fileName) {
		if(fileName == null || fileName.trim().length() == 0) {
			return;
		}
		try {
			// open the output stream and write the World to it
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
	
	/**
	 * Outputs the World to the specified output stream.
	 * 
	 * @param out the output stream to Write the World to.
	 * @throws IOException if there was an error writing to the output file.
	 */
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

	/**
	 * Renders the World onto the specified Graphics component.
	 * Currently only renders the collision boundaries and not the Entities contained within the World.
	 * 
	 * @param g the Graphics component to render onto.
	 * @param lineColour the colour to render the Edges of the Graph.
	 * @param vertexColour the colour to render the Vertices of each Edge.
	 */
	public void paintOn(Graphics g, Color lineColour, Color vertexColour) {
		graph.paintOn(g, lineColour, vertexColour);
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(Object o) {
		if(o == null || !(o instanceof World)) {
			return false;
		}
		
		World w = (World) o;
		
		// compare the object collections
		if(this.objects.size() != w.objects.size()) {
			return false;
		}
		for(int i=0;i<this.objects.size();i++) {
			if(!w.objects.contains(this.objects.elementAt(i))) {
				return false;
			}
		}
		
		// compare the tile collections
		if(this.tiles.size() != w.tiles.size()) {
			return false;
		}
		for(int i=0;i<this.tiles.size();i++) {
			if(!w.tiles.contains(this.tiles.elementAt(i))) {
				return false;
			}
		}
		
		// compare the ai collections
		if(this.ai.size() != w.ai.size()) {
			return false;
		}
		for(int i=0;i<this.ai.size();i++) {
			if(!w.ai.contains(this.ai.elementAt(i))) {
				return false;
			}
		}
		
		// compare the item collections
		if(this.items.size() != w.items.size()) {
			return false;
		}
		for(int i=0;i<this.items.size();i++) {
			if(!w.items.contains(this.items.elementAt(i))) {
				return false;
			}
		}
		
		// compare the players
		if(this.player == null && w.player != null ||
		   this.player != null && w.player == null) {
			return false;
		}
		if(this.player != null && w.player != null &&
		   !(this.player.equals(w.player))) {
			return false;
		}
		
		// compare the pets
		if(this.pet == null && w.pet != null ||
		   this.pet != null && w.pet == null) {
			return false;
		}
		if(this.pet != null && w.pet != null &&
		   !(this.pet.equals(w.pet))) {
			return false;
		}
		
		// compare the graph
		return this.graph.equals(w.graph);
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return "World";
	}
	
}
