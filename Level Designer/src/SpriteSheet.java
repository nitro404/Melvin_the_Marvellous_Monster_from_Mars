import java.awt.Dimension;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.util.Vector;

public class SpriteSheet {
	
	private Vector<Sprite> sprites;
	private String name;
	
	public SpriteSheet(BufferedImage image,
					   Vector<Rectangle> offsets) {
		this.name = null;
		sprites = new Vector<Sprite>((offsets.size() <= 1) ? 10 : offsets.size());
		
		if(image == null) {
			System.out.println("ERROR: SpriteSheet cannot be parsed from null image.");
			System.exit(1);
		}
		
		Rectangle o = null;
		for(int i=0;i<offsets.size();i++) {
			o = offsets.elementAt(i);
			if(o != null) {
				sprites.add(new Sprite(image.getSubimage(o.x, o.y, o.width, o.height), i));
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
		this.name = null;
		sprites = new Vector<Sprite>((numberOfSprites <= 1) ? 10 : numberOfSprites);
		
		if(image == null) {
			System.out.println("ERROR: SpriteSheet cannot be parsed from null image.");
			System.exit(1);
		}
		
		for(int i=0;i<numberOfSprites;i++) {
			sprites.add(new Sprite(image.getSubimage(xOffset + (xIncrement * i),
													 yOffset + (yIncrement * i),
													 width,
													 height), i));
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
		this.name = null;
		int size = numberOfRows * numberOfColumns;
		sprites = new Vector<Sprite>((size <= 1) ? 10 : size);
		
		if(image == null) {
			System.out.println("ERROR: SpriteSheet cannot be parsed from null image.");
			System.exit(1);
		}
		
		int xPos = xOffset;
		int yPos = yOffset;
		int k = 0;
		for(int i=0;i<numberOfRows;i++) {
			for(int j=0;j<numberOfColumns;j++) {
				sprites.add(new Sprite(image.getSubimage(xPos, yPos, width, height), k++));
				if(horizontal) { xPos += xIncrement; }
				else { yPos += yIncrement; }
			}
			if(horizontal) { yPos += yIncrement; xPos = xOffset; }
			else { xPos += xIncrement; yPos = yOffset; }
		}
	}
	
	public SpriteSheet(Sprite image, String name, Vector<Rectangle> offsets) {
		this(image.getImage(), offsets);
	}
	
	public SpriteSheet(Sprite image, String name, int xOffset, int yOffset, int width, int height, int xIncrement, int yIncrement, int numberOfSprites) {
		this(image.getImage(), xOffset, yOffset, width, height, xIncrement, yIncrement, numberOfSprites);
	}
	
	public SpriteSheet(Sprite image, String name, int xOffset, int yOffset, int width, int height, int xIncrement, int yIncrement, boolean horizontal, int numberOfRows, int numberOfColumns) {
		this(image.getImage(), xOffset, yOffset, width, height, xIncrement, yIncrement, horizontal, numberOfRows, numberOfColumns);
	}
	
	int size() {
		return this.sprites.size();
	}
	
	Sprite elementAt(int index) {
		return this.getSprite(index);
	}
	
	Sprite getSprite(int index) {
		if(index < 0 || index >= sprites.size()) { return null; }
		return sprites.elementAt(index);
	}
	
	Sprite getSprite(String name) {
		if(name == null || name.trim().length() == 0) { return null; }
		String temp = name.trim();
		
		for(int i=0;i<sprites.size();i++) {
			if(this.sprites.elementAt(i).getName() != null &&
			   this.sprites.elementAt(i).getName().equalsIgnoreCase(temp)) {
				return this.sprites.elementAt(i);
			}
		}
		return null;
	}

	Vector<Sprite> getSprites(int startIndex, int endIndex) {
		if(startIndex < 0 || endIndex >= sprites.size()) { return null; }
		Vector<Sprite> spriteGroup = new Vector<Sprite>(endIndex - startIndex + 1);
		for(int i=startIndex;i<=endIndex;i++) {
			spriteGroup.add(sprites.elementAt(i));
		}
		return spriteGroup;
	}
	
	public String getName() { return this.name; }
	
	public void setName(String name) { this.name = name; }
	
	public static SpriteSheet parseFrom(BufferedReader in, String spriteDirectory) {
		if(in == null || spriteDirectory == null) {
			System.out.println("ERROR: Cannot parse sprite sheet from null parameter(s).");
			return null;
		}
		
		String input;
		String data;
		Variable v;
		Variables properties = new Variables();
		Variables spriteAttributes = new Variables();
		String spriteSheetName, spriteSheetFileName;
		File spriteSheetFile;
		Sprite spriteSheetImage;
		int spriteSheetType;
		Point offset;
		Dimension size, increment;
		boolean horizontal;
		int numberOfRows, numberOfColumns, numberOfAttributes;
		SpriteSheet spriteSheet;
		try {
			while((input = in.readLine()) != null) {
				data = input.trim();
				
				if(data.length() == 0) {
					continue;
				}
				else {
					v = Variable.parseFrom(input);
					
					if(v != null) {
						properties.add(v);
					}
					
					if(v != null && v.getID().equalsIgnoreCase("Attributes")) {
						spriteSheetName = properties.getValue("SpriteSheet Name");
						if(spriteSheetName == null) {
							System.out.println("ERROR: Sprite sheet must have a name.");
							return null;
						}
						
						try { spriteSheetType = Integer.parseInt(properties.getValue("SpriteSheet Type")); }
						catch(NumberFormatException e) {
							System.out.println("ERROR: Invalid sprite sheet type.");
							return null;
						}
						
						spriteSheetFileName = properties.getValue("File Name");
						if(spriteSheetFileName == null) {
							System.out.println("ERROR: Invalid or missing sprite sheet image file name.");
							return null;
						} 
						spriteSheetFile = new File(spriteDirectory +
												   ((spriteSheetFileName.charAt(spriteSheetFileName.length() - 1) != '\\' &&
												     spriteSheetFileName.charAt(spriteSheetFileName.length() - 1) != '/') ? "\\" : "") +
												   spriteSheetFileName);
						if(!spriteSheetFile.exists() || !spriteSheetFile.isFile()) {
							System.out.println("ERROR: Invalid or missing sprite sheet image file name.");
							return null;
						}
						spriteSheetImage = new Sprite(spriteSheetFileName, spriteDirectory);
						spriteSheetImage.setType(Sprite.TYPE_SHEET);
						
						try { numberOfAttributes = Integer.parseInt(v.getValue()); }
						catch(NumberFormatException e) {
							System.out.println("ERROR: Invalid number of sprite attributes.");
							return null;
						}
						
						if(spriteSheetType == 1) { 
							System.out.println("ERROR: Sprite sheet type 1 not implemented yet.");
							return null;
						}
						if(spriteSheetType == 2) {
							System.out.println("ERROR: Sprite sheet type 2 not implemented yet.");
							return null;
						}
						if(spriteSheetType == 3) {
							offset = Utilities.parsePoint(properties.getValue("Offset"));
							if(offset == null) {
								System.out.println("ERROR: Invalid or missing sprite sheet offset.");
								return null;
							}
							
							size = Utilities.parseDimension(properties.getValue("Size"));
							if(size == null) {
								System.out.println("ERROR: Invalid or missing sprite sheet tile size.");
								return null;
							}
							
							increment = Utilities.parseDimension(properties.getValue("Increment"));
							if(increment == null) {
								System.out.println("ERROR: Invalid or missing sprite sheet tile increment.");
								return null;
							}
							
							String temp = properties.getValue("Horizontal");
							if(temp == null) {
								System.out.println("ERROR: Missing horizontal / vertical sprite sheet specification.");
								return null;
							}
							horizontal = temp.equalsIgnoreCase("true");
							
							try { numberOfRows = Integer.parseInt(properties.getValue("Number of Rows")); }
							catch(NumberFormatException e) {
								System.out.println("ERROR: Missing number of rows in sprite sheet.");
								return null;
							}
							
							try { numberOfColumns = Integer.parseInt(properties.getValue("Number of Columns")); }
							catch(NumberFormatException e) {
								System.out.println("ERROR: Missing number of columns in sprite sheet.");
								return null;
							}
							
							spriteSheet = new SpriteSheet(spriteSheetImage.getImage(),
														  offset.x,
														  offset.y,
														  size.width,
														  size.height,
														  increment.width,
														  increment.height,
														  horizontal,
														  numberOfRows,
														  numberOfColumns);
							spriteSheet.setName(spriteSheetName);
						}
						else {
							System.out.println("ERROR: Invalid sprite sheet type specified.");
							return null;
						}
						
						int spriteIndex;
						String spriteName, spriteType;
						spriteAttributes = new Variables();
						for(int i=0;i<numberOfAttributes;i++) {
							for(int j=0;j<3;j++) {
								spriteAttributes.add(Variable.parseFrom(in.readLine()));
							}
							try { spriteIndex = Integer.parseInt(spriteAttributes.getValue("Sprite")); }
							catch(NumberFormatException e) {
								System.out.println("ERROR: Invalid sprite attribute index.");
								return null;
							}
							if(spriteIndex < 0 || spriteIndex >= spriteSheet.sprites.size()) {
								System.out.println("ERROR: Sprite attribute index out of range.");
								return null;
							}
							spriteName = spriteAttributes.getValue("Name");
							spriteType = spriteAttributes.getValue("Type");
							if(spriteName == null || spriteType == null) {
								System.out.println("ERROR: Sprite name or type missing.");
								return null;
							}
							spriteSheet.sprites.elementAt(spriteIndex).setName(spriteName);
							spriteSheet.sprites.elementAt(spriteIndex).setParentName(spriteSheetName);
							spriteSheet.sprites.elementAt(spriteIndex).setType(Sprite.parseType(spriteType));
							
							spriteAttributes.clear();
						}
						
						return spriteSheet;
					}
				}
			}
		}
		catch(IOException e ) {
			System.out.println("ERROR: Error reading sprite sheet file.");
			return null;
		}
		catch(Exception e) {
			e.printStackTrace();
			System.out.println("ERROR: Unknown exception encountered parsing sprite sheet file.");
			return null;
		}
		return null;
	}
	
}
