import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Vector;

public class SpriteSheets {
	
	Vector<SpriteSheet> spriteSheets;
	
	public SpriteSheets() {
		this.spriteSheets = new Vector<SpriteSheet>(5);
	}
	
	public int size() {
		return this.spriteSheets.size();
	}
	
	public SpriteSheet elementAt(int index) {
		return this.getSpriteSheet(index);
	}
	
	public SpriteSheet getSpriteSheet(int index) {
		if(index < 0 || index >= this.spriteSheets.size()) { return null; }
		
		return this.spriteSheets.elementAt(index);
	}

	public SpriteSheet getSpriteSheet(String name) {
		if(name == null || name.trim().length() == 0) { return null; }
		String temp = name.trim();
		
		for(int i=0;i<this.spriteSheets.size();i++) {
			if(this.spriteSheets.elementAt(i).getName() != null &&
			   this.spriteSheets.elementAt(i).getName().equalsIgnoreCase(temp)) {
				return this.spriteSheets.elementAt(i);
			}
		}
		return null;
	}
	
	public int getSpriteSheetIndex(String name) {
		if(name == null || name.trim().length() == 0) { return -1; }
		String temp = name.trim();
		
		for(int i=0;i<this.spriteSheets.size();i++) {
			if(this.spriteSheets.elementAt(i).getName() != null &&
			   this.spriteSheets.elementAt(i).getName().equalsIgnoreCase(temp)) {
				return i;
			}
		}
		return -1;
	}
	
	public Sprite getSprite(String name) {
		if(name == null || name.trim().length() == 0) { return null; }
		String temp = name.trim();
		
		for(int i=0;i<this.spriteSheets.size();i++) {
			for(int j=0;j<this.spriteSheets.elementAt(i).size();j++) {
				if(this.spriteSheets.elementAt(i).elementAt(j).getName() != null &&
				   temp.equalsIgnoreCase(this.spriteSheets.elementAt(i).elementAt(j).getName())) {
					return this.spriteSheets.elementAt(i).elementAt(j);
				}
			}
		}
		return null;
	}
	
	public boolean add(SpriteSheet spriteSheet) {
		if(spriteSheet == null) { return false; }
		
		if(!this.spriteSheets.contains(spriteSheet)) {
			this.spriteSheets.add(spriteSheet);
			return true;
		}
		return false;
	}
	
	public static SpriteSheets parseFrom(String fileName, String spriteDirectory) {
		if(fileName == null || spriteDirectory == null) {
			System.out.println("ERROR: Cannot parse sprite sheet collection from null parameter(s).");
			return null;
		}
		
		File file = new File(fileName);
		File dir = new File(spriteDirectory);
		if(!file.exists() || !file.isFile() || !dir.exists() || !dir.isDirectory()) {
			System.out.println("ERROR: Invalid or missing sprite sheet file or sprite sheet directory.");
			return null;
		}
		
		BufferedReader in;
		SpriteSheet spriteSheet;
		SpriteSheets spriteSheets = new SpriteSheets();
		try {
			in = new BufferedReader(new FileReader(file));
			
			do {
				spriteSheet = SpriteSheet.parseFrom(in, spriteDirectory);
				if(spriteSheet != null) {
					spriteSheets.add(spriteSheet);
				}
			} while(spriteSheet != null);
			
			in.close();
		}
		catch(IOException e) {
			System.out.println("ERROR: Error reading sprite sheet file.");
			return null;
		}
		
		return spriteSheets;
	}
	
}