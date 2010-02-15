import java.io.*;
import java.util.StringTokenizer;

public class Entity {

	public Vertex location;
	public int textureIndex;
	
	public Entity(Vertex location, int textureIndex) {
		this.location = location;
		this.textureIndex = textureIndex;
	}
	
	public static Entity parseFrom(String input) {
		if(input == null || input.trim().length() == 0) {
			return null;
		}
		
		String data = input.trim();
		
		StringTokenizer st = new StringTokenizer(data, ", ", false);
		Vertex v = new Vertex(Integer.valueOf(st.nextToken()), Integer.valueOf(st.nextToken()));
		int textureIndex = Integer.valueOf(st.nextToken());
		
		return new Entity(v, textureIndex);
	}
	
	public void writeTo(PrintWriter out) {
		this.location.writeTo(out);
		out.print(", " + textureIndex);
	}
	
	public boolean equals(Object o) {
		if(o == null || !(o instanceof Entity)) {
			return false;
		}
		
		Entity e = (Entity) o;
		
		return this.location.equals(e.location);
	}
	
	public String toString() {
		return this.location.toString();
	}
	
}
