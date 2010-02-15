import java.io.*;
import java.awt.*;

public class Vertex {
	
	public int x, y;
	
	public Vertex(int x, int y) {
		this.x = x;
		this.y = y;
	}
	
	public Vertex(Point p) {
		this.x = p.x;
		this.y = p.y;
	}
	
	public int getX() {
		return this.x;
	}
	
	public int getY() {
		return this.y;
	}
	
	public void setLocation(int x, int y) {
		this.x = x;
		this.y = y;
	}
	
	public Point toPoint() {
		return new Point(x, y);
	}
	
	public static Vertex parseFrom(String input) {
		if(input == null || input.trim().length() == 0) {
			return null;
		}
		
		String data = input.trim();
		
		int x = Integer.valueOf(data.substring(0, data.indexOf(',')).trim());
		int y = Integer.valueOf(data.substring(data.lastIndexOf(',', data.length() - 1) + 1, data.length()).trim());
		
		return new Vertex(x, y);
	}
	
	public void writeTo(PrintWriter out) {
		out.print(x + ", " + y);
	}
	
	public void paintOn(Graphics g) {
		int radius = 2;
		if(EditorPanel.gridTopIsometric != null) {
			g.fillOval(World.getIsometricX(x, y) - radius, 
					   World.getIsometricY(x, y) - radius,
					   radius * 2, radius * 2);
		}
	}
	
	public boolean equals(Object o) {
		if(o == null || !(o instanceof Vertex)) {
			return false;
		}
		
		Vertex p = (Vertex) o;
		
		return this.x == p.x && this.y == p.y;
	}
	
	public String toString() {
		return "(" + this.x + ", " + this.y + ")";
	}
}
