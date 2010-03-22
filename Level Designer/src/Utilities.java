import java.awt.Color;
import java.awt.Dimension;
import java.awt.Point;
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
	
}
