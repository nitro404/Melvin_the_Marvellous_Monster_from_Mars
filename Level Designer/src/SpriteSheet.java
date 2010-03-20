import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.util.Vector;

public class SpriteSheet {
	
	private Vector<Sprite> sprites;
	
	public SpriteSheet(BufferedImage image,
					   Vector<Rectangle> offsets) {
		sprites = new Vector<Sprite>((offsets.size() <= 1) ? 10 : offsets.size());
		
		if(image == null) {
			System.out.println("ERROR: SpriteSheet cannot be parsed from null image.");
			System.exit(1);
		}
		
		Rectangle o = null;
		for(int i=0;i<offsets.size();i++) {
			o = offsets.elementAt(i);
			if(o != null) {
				sprites.add(new Sprite(image.getSubimage(o.x, o.y, o.width, o.height)));
			}
		}
	}
	
	public SpriteSheet(BufferedImage image,
					   int xOffset,
					   int yOffset,
					   int width,
					   int height,
					   int xIncrement,
					   int yIncrement,
					   int numberOfSprites) {
		sprites = new Vector<Sprite>((numberOfSprites <= 1) ? 10 : numberOfSprites);
		
		if(image == null) {
			System.out.println("ERROR: SpriteSheet cannot be parsed from null image.");
			System.exit(1);
		}
		
		for(int i=0;i<numberOfSprites;i++) {
			sprites.add(new Sprite(image.getSubimage(xOffset + (xIncrement * i),
													 yOffset + (yIncrement * i),
													 width,
													 height)));
		}
	}
	
	public SpriteSheet(BufferedImage image,
					   int xOffset,
					   int yOffset,
					   int width,
					   int height,
					   int xIncrement,
					   int yIncrement,
					   boolean horizontal,
					   int numberOfRows,
					   int numberOfColumns) {
		int size = numberOfRows * numberOfColumns;
		sprites = new Vector<Sprite>((size <= 1) ? 10 : size);
		
		if(image == null) {
			System.out.println("ERROR: SpriteSheet cannot be parsed from null image.");
			System.exit(1);
		}
		
		int xPos = xOffset;
		int yPos = yOffset;
		for(int i=0;i<numberOfRows;i++) {
			for(int j=0;j<numberOfColumns;j++) {
				sprites.add(new Sprite(image.getSubimage(xPos, yPos, width, height)));
				if(horizontal) { xPos += xIncrement; }
				else { yPos += yIncrement; }
			}
			if(horizontal) { yPos += yIncrement; xPos = xOffset; }
			else { xPos += xIncrement; yPos = yOffset; }
		}
	}
	
	public SpriteSheet(Sprite image, Vector<Rectangle> offsets) {
		this(image.getImage(), offsets);
	}
	
	public SpriteSheet(Sprite image, int xOffset, int yOffset, int width, int height, int xIncrement, int yIncrement, int numberOfSprites) {
		this(image.getImage(), xOffset, yOffset, width, height, xIncrement, yIncrement, numberOfSprites);
	}
	
	public SpriteSheet(Sprite image, int xOffset, int yOffset, int width, int height, int xIncrement, int yIncrement, boolean horizontal, int numberOfRows, int numberOfColumns) {
		this(image.getImage(), xOffset, yOffset, width, height, xIncrement, yIncrement, horizontal, numberOfRows, numberOfColumns);
	}
	
	Sprite getSprite(int index) {
		if(index < 0 || index >= sprites.size()) { return null; }
		return sprites.elementAt(index);
	}

	Vector<Sprite> getSprites(int startIndex, int endIndex) {
		if(startIndex < 0 || endIndex >= sprites.size()) { return null; }
		Vector<Sprite> spriteGroup = new Vector<Sprite>(endIndex - startIndex + 1);
		for(int i=startIndex;i<=endIndex;i++) {
			spriteGroup.add(sprites.elementAt(i));
		}
		return spriteGroup;
	}
	
}
