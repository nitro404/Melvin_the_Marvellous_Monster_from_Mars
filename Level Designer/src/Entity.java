import java.awt.Graphics;
import java.io.*;
import java.util.StringTokenizer;

public class Entity {

	public Vertex location;
	public Sprite sprite;
	public int spriteSheetIndex;
	
	public Entity(Vertex location, Sprite sprite) {
		this.location = location;
		this.sprite = sprite;
		this.spriteSheetIndex = -1;
	}
	
	public Vertex getPosition() {
		return this.location;
	}
	
	public int getSpriteIndex() {
		return (this.sprite == null) ? -1 : this.sprite.getIndex();
	}
	
	public boolean isTiled() {
		return (this.sprite == null) ? false : this.sprite.isTiled();
	}
	
	public Sprite getSprite() {
		return this.sprite;
	}
	
	public int getType() {
		return (this.sprite == null) ? Sprite.TYPE_UNKNOWN : this.sprite.getType();
	}
	
	public int getWidth() {
		return (this.sprite == null) ? -1 : this.sprite.getWidth();
	}
	
	public int getHeight() {
		return (this.sprite == null) ? -1 : this.sprite.getWidth();
	}
	
	public void paintOn(Graphics g) {
		if(sprite.isTiled()) {
			sprite.paintOn(g, location.x * World.GRID_SIZE, location.y * World.GRID_SIZE);
		}
		else {
			sprite.paintOn(g, location.x, location.y);
		}
	}
	
	public static Entity parseFrom(String input, SpriteSheets spriteSheets) {
		if(input == null || input.trim().length() == 0) {
			return null;
		}
		
		String data = input.trim();
		
		StringTokenizer st = new StringTokenizer(data, ",", false);
		if(st.countTokens() < 2 || st.countTokens() > 4) { return null; }
		Vertex v = new Vertex(Integer.valueOf(st.nextToken().trim()), Integer.valueOf(st.nextToken().trim()));
		Entity newEntity = null;
		SpriteSheet spriteSheet;
		Sprite sprite;
		if(st.countTokens() == 2) {
			String spriteSheetName = st.nextToken().trim();
			String spriteName = st.nextToken().trim();
			
			spriteSheet = spriteSheets.getSpriteSheet(spriteSheetName);
			if(spriteSheet == null) {
				System.out.println("ERROR: Unable to load entity sprite \"" + spriteName + "\" from sprite sheet \"" + spriteSheetName + "\".");
				return null;
			}
			sprite = spriteSheet.getSprite(spriteName);
			if(sprite == null) {
				System.out.println("ERROR: Unable to load entity sprite \"" + spriteName + "\" from sprite sheet \"" + spriteSheetName + "\".");
				return null;
			}
			newEntity = new Entity(v, sprite);
			newEntity.spriteSheetIndex = spriteSheets.getSpriteSheetIndex(spriteSheetName);
		}
		else {
			newEntity = new Entity(v, null);
		}
		return newEntity;
	}
	
	public void writeTo(PrintWriter out) {
		this.location.writeTo(out);
		if(sprite != null) {
			out.print(", " + sprite.getParentName() + ", " + sprite.getName());
		}
	}
	
	public boolean equals(Object o) {
		if(o == null || !(o instanceof Entity)) {
			return false;
		}
		
		Entity e = (Entity) o;
		
		if((this.sprite == null && e.sprite != null) ||
		   (this.sprite != null && e.sprite == null) ||
		   (this.sprite != null && e.sprite != null && !this.sprite.equals(e.sprite))) {
			return false;
		}
		
		return this.location.equals(e.location);
	}
	
	public String toString() {
		return this.location.toString();
	}
	
}
