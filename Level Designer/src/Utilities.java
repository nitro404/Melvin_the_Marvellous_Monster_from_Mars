import java.awt.Color;
import java.awt.Dimension;
import java.awt.Point;
import java.io.File;
import java.util.StringTokenizer;

public class Utilities {

	public static Dimension parseDimension(String data) {
		if(data != null && data.trim().length() > 0) {
			StringTokenizer st = new StringTokenizer(data.trim(), ",x ", false);
			if(st.countTokens() == 2) {
				try {
					return new Dimension(Integer.parseInt(st.nextToken()), Integer.parseInt(st.nextToken()));
				}
				catch(NumberFormatException e) { }
			}
		}
		return null;
	}
	
	public static Point parsePoint(String data) {
		if(data != null && data.trim().length() > 0) {
			StringTokenizer st = new StringTokenizer(data.trim(), ", ", false);
			if(st.countTokens() == 2) {
				try {
					return new Point(Integer.parseInt(st.nextToken()), Integer.parseInt(st.nextToken()));
				}
				catch(NumberFormatException e) { }
			}
		}
		return null;
	}
	
	public static Color parseColour(String data) {
		if(data != null && data.trim().length() > 0) {
			StringTokenizer st = new StringTokenizer(data.trim(), ", ", false);
			if(st.countTokens() == 3) {
				try {
					return new Color(Integer.parseInt(st.nextToken()),
							         Integer.parseInt(st.nextToken()),
							         Integer.parseInt(st.nextToken()));
				}
				catch(NumberFormatException e) { }
			}
		}
		return null;
	}
	
	public static boolean isValidFile(String fileName) {
		if(fileName == null || fileName.trim().length() == 0) { return false; }
		File file = new File(fileName.trim());
		if(!file.exists() || !file.isFile()) { return false; }
		return true;
	}
	
	public static boolean isValidDirectory(String fileName) {
		if(fileName == null || fileName.trim().length() == 0) { return false; }
		File file = new File(fileName.trim());
		if(!file.exists() || !file.isDirectory()) { return false; }
		return true;
	}
	
	public static String getValidFile(String initialFileName, String defaultFileName, String alternateFileName) {
		String fileName = Utilities.isValidFile(initialFileName) ? initialFileName : null;
		if(fileName == null) { fileName = Utilities.isValidFile(defaultFileName) ? defaultFileName : null; }
		if(fileName == null) { fileName = Utilities.isValidFile(alternateFileName) ? alternateFileName : null; }
		if(fileName == null) { fileName = defaultFileName; }
		return fileName;
	}
	
	public static String getValidDirectory(String initialFileName, String defaultFileName, String alternateFileName) {
		String fileName = Utilities.isValidDirectory(initialFileName) ? initialFileName : null;
		if(fileName == null) { fileName = Utilities.isValidDirectory(defaultFileName) ? defaultFileName : null; }
		if(fileName == null) { fileName = Utilities.isValidDirectory(alternateFileName) ? alternateFileName : null; }
		if(fileName == null) { fileName = defaultFileName; }
		return fileName;
	}
	
	public static String getExtension(String fileName) {
		if(fileName.lastIndexOf('.') != -1) {
			return fileName.substring(fileName.lastIndexOf('.') + 1, fileName.length());
		}
		else {
			return null;
		}
	}
	
	public static boolean hasExtension(String fileName, String fileExtension) {
		String actualFileExtension = getExtension(fileName);
		return actualFileExtension != null && actualFileExtension.equalsIgnoreCase(fileExtension);
	}
	
	public static String removeExtension(String fileName) {
		if(fileName.lastIndexOf('.') != -1) {
			return fileName.substring(0, fileName.lastIndexOf('.'));
		}
		else {
			return fileName;
		}
	}
	
}
