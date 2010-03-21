import java.io.*;
import java.util.StringTokenizer;

public class Entity {

	public Vertex location;
	public int textureIndex;
	public int spriteIndex;
	
	public Entity(Vertex location, int textureIndex, int spriteIndex) {
		this.location = location;
		this.textureIndex = textureIndex;
		this.spriteIndex = spriteIndex;
	}
	
	public Vertex getPosition() {
		return this.location;
	}
	
	public static Entity parseFrom(String input) {
		if(input == null || input.trim().length() == 0) {
			return null;
		}
		
		String data = input.trim();
		
		StringTokenizer st = new StringTokenizer(data, ", ", false);
		Vertex v = new Vertex(Integer.valueOf(st.nextToken()), Integer.valueOf(st.nextToken()));
		int textureIndex = Integer.valueOf(st.nextToken());
		int spriteIndex = Integer.valueOf(st.nextToken());
		
		return new Entity(v, textureIndex, spriteIndex);
	}
	
	public void writeTo(PrintWriter out) {
		this.location.writeTo(out);
		out.print(", " + textureIndex + ", " + spriteIndex);
	}
	
	public boolean equals(Object o) {
		if(o == null || !(o instanceof Entity)) {
			return false;
		}
		
		Entity e = (Entity) o;
		
		return this.textureIndex == e.textureIndex &&
			   this.spriteIndex == e.spriteIndex &&
			   this.location.equals(e.location);
	}
	
	public String toString() {
		return this.location.toString();
	}
	
}
