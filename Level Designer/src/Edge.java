import java.util.StringTokenizer;
import java.io.*;
import java.awt.*;

public class Edge {
	
	public Vertex a, b;
	
	public Edge(Vertex a, Vertex b) {
		this.a = a;
		this.b = b;
	}
	
	public Vertex getPointA() {
		return this.a;
	}
	
	public Vertex getPointB() {
		return this.b;
	}
	
	public boolean containsVertex(Vertex p) {
		return this.a == p || this.b == p;
	}
	
	public double getDeltaX() {
		return this.b.x - this.a.x;
	}

	public double getDeltaY() {
		return this.b.x - this.a.x;
	}
	
	public static Edge parseFrom(String input) {
		if(input == null || input.trim().length() == 0) {
			return null;
		}
		
		String data = input.trim();
		
		StringTokenizer st = new StringTokenizer(data, ", ", false);
		Vertex a = Vertex.parseFrom(st.nextToken() + ", " + st.nextToken());
		Vertex b = Vertex.parseFrom(st.nextToken() + ", " + st.nextToken());
		
		return new Edge(a, b);
	}
	
	public void writeTo(PrintWriter out) throws IOException {
		a.writeTo(out);
		out.print(", ");
		b.writeTo(out);
	}
	
	public void paintOn(Graphics g) {
		g.drawLine(World.getIsometricX(a.x, a.y),
				   World.getIsometricY(a.x, a.y),
				   World.getIsometricX(b.x, b.y),
				   World.getIsometricY(b.x, b.y));
	}
	
	public boolean equals(Object o) {
		if(o == null || !(o instanceof Edge)) {
			return false;
		}
		
		Edge e = (Edge) o;
		
		return this.a == e.a && this.b == e.b;
	}
	
	public String toString() {
		return "(" + this.a.x + ", " + this.a.y + ") (" + this.b.x + ", " + this.b.y + ")";
	}
	
}
