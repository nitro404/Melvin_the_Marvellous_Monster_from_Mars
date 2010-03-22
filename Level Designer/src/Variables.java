import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Vector;

public class Variables {
	
	private Vector<Variable> variables;
	
	public Variables() {
		this.variables = new Vector<Variable>(10);
	}
	
	public boolean add(String id, String value) {
		return this.add(new Variable(id, value));
	}
	
	public boolean add(Variable v) {
		if(v != null && v.getID().length() > 0 && !this.variables.contains(v)) {
			this.variables.add(v);
			return true;
		}
		return false;
	}
	
	public void add(Variables v) {
		if(v == null) { return; }
		
		for(int i=0;i<v.variables.size();i++) {
			this.add(v.variables.elementAt(i));
		}
	}
	
	public void add(Vector<Variable> v) {
		if(v == null) { return; }
		
		for(int i=0;i<v.size();i++) {
			this.add(v.elementAt(i));
		}
	}
	
	public boolean remove(int index) {
		if(index < 0 || index >= this.variables.size()) { return false; }
		
		this.variables.remove(index);
		
		return true;
	}
	
	public boolean remove(String id) {
		if(id == null || id.trim().length() < 1) { return false; }
		String temp = id.trim();
		
		for(int i=0;i<this.variables.size();i++) {
			if(this.variables.elementAt(i).getID().equalsIgnoreCase(temp)) {
				this.variables.remove(i);
				return true;
			}
		}
		
		return false;
	}
	
	public boolean remove(Variable v) {
		if(v == null) { return false; }
		
		for(int i=0;i<this.variables.size();i++) {
			if(this.variables.elementAt(i).equals(v)) {
				this.variables.remove(i);
				return true;
			}
		}
		
		return false;
	}
	
	public void clear() {
		this.variables.clear();
	}
	
	public int size() {
		return this.variables.size();
	}
	
	public boolean contains(String id) {
		if(id == null || id.trim().length() < 1) { return false; }
		String temp = id.trim();
		
		for(int i=0;i<this.variables.size();i++) {
			if(this.variables.elementAt(i).getID().equalsIgnoreCase(temp)) {
				return true;
			}
		}
		
		return false;
	}
	
	public boolean contains(Variable v) {
		if(v == null) { return false; }
		
		for(int i=0;i<this.variables.size();i++) {
			if(this.variables.elementAt(i).equals(v)) {
				return true;
			}
		}
		
		return false;
	}
	
	public int indexOf(String id) {
		if(id == null || id.trim().length() < 1) { return -1; }
		String temp = id.trim();
		
		for(int i=0;i<this.variables.size();i++) {
			if(this.variables.elementAt(i).getID().equalsIgnoreCase(temp)) {
				return i;
			}
		}
		
		return -1;
	}
	
	public int indexOf(Variable v) {
		if(v == null) { return -1; }
		
		for(int i=0;i<this.variables.size();i++) {
			if(this.variables.elementAt(i).equals(v)) {
				return i;
			}
		}
		
		return -1;
	}
	
	public Variable elementAt(int index) {
		if(index < 0 || index >= this.variables.size()) { return null; }
		
		return this.variables.elementAt(index);
	}
	
	public String getValue(String id) {
		if(id == null || id.trim().length() < 1) { return null; }
		String temp = id.trim();
		
		for(int i=0;i<this.variables.size();i++) {
			if(this.variables.elementAt(i).getID().equalsIgnoreCase(temp)) {
				return this.variables.elementAt(i).getValue();
			}
		}
		
		return null;
	}
	
	public boolean parseFrom(String fileName) {
		return parseFrom(new File(fileName), false);
	}
	
	public boolean parseFrom(String fileName, boolean append) {
		return parseFrom(new File(fileName), append);
	}
	
	public boolean parseFrom(File file) {
		return parseFrom(file, false);
	}
	
	public boolean parseFrom(File file, boolean append) {
		if(file == null || !file.exists() || !file.isFile()) { return false; }
		
		BufferedReader in;
		String input;
		try {
			in = new BufferedReader(new FileReader(file));
			
			if(!append) {
				this.variables = new Vector<Variable>(10);
			}
			
			while((input = in.readLine()) != null) {
				Variable v = Variable.parseFrom(input);
				
				if(v != null) {
					if(this.contains(v)) {
						this.remove(v);
					}
					
					this.add(v);
				}
			}
			
			in.close();
		}
		catch(IOException e) {
			return false;
		}
		
		return true;
	}
	
	public boolean writeTo(String fileName) {
		return this.writeTo(new File(fileName));
	}
	
	public boolean writeTo(File file) {
		if(file == null || !file.exists() || !file.isFile()) { return false; }
		
		PrintWriter out;
		try {
			out = new PrintWriter(new FileWriter(file));
			this.writeTo(out);
			out.close();
		}
		catch(IOException e) {
			return false;
		}
		return true;
	}
	
	public void writeTo(PrintWriter out) {
		for(int i=0;i<this.variables.size();i++) {
			this.variables.elementAt(i).writeTo(out);
		}
	}
	
	public boolean equals(Object o) {
		if(o == null || !(o instanceof Variables)) { return false; }
		
		Variables v = (Variables) o;
		
		if(this.variables.size() != v.variables.size()) { return false; }
		
		for(int i=0;i<this.variables.size();i++) {
			if(!v.contains(this.variables.elementAt(i))) {
				return false;
			}
		}
		return true;
	}
	
	public String toString() {
		String s = new String();
		
		for(int i=0;i<this.variables.size();i++) {
			s += this.variables.elementAt(i);
			if(i<this.variables.size()-1) {
				s += ", ";
			}
		}
		
		return s;
	}
	
}
