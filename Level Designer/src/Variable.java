import java.io.PrintWriter;

public class Variable {

	private String id;
	private String value;
	
	final private static char separatorChar = ':';
	
	public Variable(String id, String value) {
		this.id = (id == null) ? "" : id;
		this.value = (value == null) ? "" : value;
	}
	
	public String getID() {
		return this.id;
	}
	
	public String getValue() {
		return this.value;
	}
	
	public void setID(String id) {
		this.id = (id == null) ? "" : id;
	}
	
	public void setValue(String value) {
		this.value = (value == null) ? "" : value;
	}
	
	public static Variable parseFrom(String data) {
		if(data == null || data.trim().length() < 1) { return null; }
		String temp = data.trim();
		
		int separatorIndex = temp.indexOf(separatorChar);
		if(separatorIndex < 1) { return null; }
		
		String newID = temp.substring(0, separatorIndex).trim();
		String newValue = temp.substring(separatorIndex + 1, temp.length()).trim();
		
		return new Variable(newID, newValue);
	}
	
	public void writeTo(PrintWriter out) {
		if(out == null) { return; }
		
		out.println(this.id + separatorChar + " " + this.value);
	}
	
	public boolean equals(Object o) {
		if(o == null || !(o instanceof Variable)) {
			return false;
		}
		
		Variable v = (Variable) o;
		
		return this.id.equalsIgnoreCase(v.id);
	}
	
	public String toString() {
		return this.id + separatorChar + " " + this.value;
	}
	
	
}
