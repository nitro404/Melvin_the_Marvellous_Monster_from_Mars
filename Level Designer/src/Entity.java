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
	
	public static Entity parseFrom(String input, SpriteSheets spriteSheets) {
		if(input == null || input.trim().length() == 0) {
			return null;
		}
		
		String data = input.trim();
		
		StringTokenizer st = new StringTokenizer(data, ", ", false);
		Vertex v = new Vertex(Integer.valueOf(st.nextToken()), Integer.valueOf(st.nextToken()));
		if(st.countTokens() == 2) {
			String spriteSheetName = st.nextToken();
			String spriteName = st.nextToken();
			
			return new Entity(v, spriteSheets.getSpriteSheet(spriteSheetName).getSprite(spriteName));
		}
		else {
			return new Entity(v, null);
		}
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
