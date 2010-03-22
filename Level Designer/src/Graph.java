import java.util.Vector;
import java.awt.Color;
import java.awt.Graphics;
import java.io.*;

public class Graph {
	
	public Vector<Edge> edges;
	public Vector<Vertex> vertices;
	
	public Graph() {
		this.edges = new Vector<Edge>();
		this.vertices = new Vector<Vertex>();
	}
	
	public void addVertex(Vertex v) {
		if(v != null && !this.vertices.contains(v)) {
			this.vertices.add(v);
		}
	}
	
	public void addEdge(Edge e) {
		if(e != null && !this.edges.contains(e)) {
			this.edges.add(e);
			if(e.a != null && !this.vertices.contains(e.a)) {
				this.vertices.add(e.a);
			}
			if(e.b != null && !this.vertices.contains(e.b)) {
				this.vertices.add(e.b);
			}
		}
	}
	
	public boolean containsEdge(Edge e) {
		if(e == null) {
			return false;
		}
		
		for(int i=0;i<this.edges.size();i++) {
			if(this.edges.elementAt(i).equals(e)) {
				return true;
			}
		}
		return false;
	}
	
	public int size() {
		return this.edges.size();
	}
	
	public void writeTo(PrintWriter out) throws IOException {
		for(int i=0;i<this.edges.size();i++) {
			out.print("\t");
			this.edges.elementAt(i).writeTo(out);
			out.println();
		}
	}
	
	public void paintOn(Graphics g) {
		g.setColor(new Color(0, 0, 0));
		
		for(int i=0;i<this.edges.size();i++) {
			this.edges.elementAt(i).paintOn(g);
		}
		for(int i=0;i<this.vertices.size();i++) {
			this.vertices.elementAt(i).paintOn(g);
		}
	}
	
	public boolean equals(Object o) {
		if(o == null || !(o instanceof Graph)) {
			return false;
		}
		
		Graph g = (Graph) o;
		
		return this.edges.equals(g.edges);
	}
	
	public String toString() {
		String s = "[";
		for(int i=0;i<this.edges.size();i++) {
			s += this.edges.elementAt(i);
			if(i < this.edges.size() - 1) {
				s += ", ";
			}
		}
		s += "]";
		return s;
	}
}