import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class Sprite {
	
	private File imageFile;
	private BufferedImage image;
	private String name;
	private String parentName;
	private int index;
	private int type;
	
	public static int TYPE_UNKNOWN = -1;
	public static int TYPE_SHEET = 0;
	public static int TYPE_TILE = 1;
	public static int TYPE_OBJECT = 2;
	public static int TYPE_PLAYER = 3;
	public static int TYPE_DISGUISE = 4;
	public static int TYPE_AI = 5;
	
	public Sprite(String fileName, String spriteDirectory) {
		this(fileName, spriteDirectory, -1);
	}
	
	public Sprite(String fileName, String spriteDirectory, int index) {
		this.name = null;
		this.parentName = null;
		this.index = index;
		this.type = TYPE_UNKNOWN;
		
		String imagePath = spriteDirectory;
		if(imagePath.charAt(imagePath.length() - 1) != '\\' && imagePath.charAt(imagePath.length() - 1) != '/') {
			imagePath += '\\';
		}
		imagePath += fileName;
		
		this.imageFile = new File(imagePath);
		
		if(!this.imageFile.exists() || !this.imageFile.isFile()) {
			System.out.println("ERROR: Sprite: \"" + this.imageFile.getPath() + "\" does not exist or is an invalid image.");
			System.exit(1);
		}
		
		try {
			this.image = ImageIO.read(this.imageFile);
		}
		catch(IOException e) {
			System.out.println("ERROR: Unable to read sprite image: \"" + this.imageFile.getPath() + "\".");
			System.exit(1);
		}
	}
	
	public Sprite(String fileName, String spriteDirectory, int x, int y, int width, int height) {
		this(fileName, spriteDirectory, -1, x, y, width, height);
	}
	
	public Sprite(String fileName, String spriteDirectory, int index, int x, int y, int width, int height) {
		this.name = null;
		this.parentName = null;
		this.index = index;
		this.type = TYPE_UNKNOWN;
		
		String imagePath = spriteDirectory;
		if(imagePath.charAt(imagePath.length() - 1) != '\\' && imagePath.charAt(imagePath.length() - 1) != '/') {
			imagePath += '\\';
		}
		imagePath += fileName;
		
		this.imageFile = new File(imagePath);
		
		if(!this.imageFile.exists() || !this.imageFile.isFile()) {
			System.out.println("ERROR: Sprite: \"" + this.imageFile.getPath() + "\" does not exist or is an invalid image.");
			System.exit(1);
		}
		
		try {
			this.image = ImageIO.read(this.imageFile).getSubimage(x, y, width, height);
		}
		catch(IOException e) {
			System.out.println("ERROR: Unable to read sprite image: \"" + this.imageFile.getPath() + "\".");
			System.exit(1);
		}
	}
	
	public Sprite(BufferedImage image) {
		this(image, -1);
	}
	
	public Sprite(BufferedImage image, int index) {
		this.name = null;
		this.parentName = null;
		this.index = index;
		this.type = TYPE_UNKNOWN;
		
		if(image == null) {
			System.out.println("ERROR: Sprite cannot be created from null image.");
			System.exit(1);
		}
		
		this.image = image;
	}
	
	public Sprite(BufferedImage image, int index, int x, int y, int width, int height) {
		this.name = null;
		this.parentName = null;
		this.index = index;
		this.type = TYPE_UNKNOWN;
		
		if(image == null) {
			System.out.println("ERROR: Sprite cannot be created from null image.");
			System.exit(1);
		}
		
		this.image = image.getSubimage(x, y, width, height);
	}
	
	public int getWidth() { return this.image.getWidth(); }
	public int getHeight() { return this.image.getHeight(); }
	
	public Dimension getDimensions() { return new Dimension(this.image.getWidth(), this.image.getHeight()); }
	
	public String getName() { return this.name; }
	public String getParentName() { return this.parentName; }
	public int getIndezx() { return this.index; }
	public int getType() { return this.type; }
	
	public void setName(String name) { this.name = name; }
	public void setParentName(String parentName) { this.parentName = parentName; }
	public void setIndex(int index) { this.index = index; }
	public void setType(int type) { this.type = type; }
	
	public BufferedImage getImage() { return this.image; }
	
	public static int parseType(String data) {
		if(data == null || data.trim().length() == 0) { return TYPE_UNKNOWN; }
		String typeString = data.trim();
		
		if(typeString.equalsIgnoreCase("SpriteSheet")) {
			return TYPE_SHEET;
		}
		else if(typeString.equalsIgnoreCase("Tile")) {
			return TYPE_TILE;
		}
		else if(typeString.equalsIgnoreCase("Object")) {
			return TYPE_OBJECT;
		}
		else if(typeString.equalsIgnoreCase("Player")) {
			return TYPE_PLAYER;
		}
		else if(typeString.equalsIgnoreCase("Disguise")) {
			return TYPE_DISGUISE;
		}
		else if(typeString.equalsIgnoreCase("AI")) {
			return TYPE_AI;
		}
		else {
			return TYPE_UNKNOWN;
		}
	}
	
	public void paintOn(Graphics g, int x, int y) {
		((Graphics2D) g).drawImage(image, null, x, y);
	}
	
	public void paintOnCentered(Graphics g, int x, int y) {
		((Graphics2D) g).drawImage(image, null, (int) (x - (image.getWidth() / 2.0)), (int) (y - (image.getHeight() / 2.0)));
	}
	
}
