public class LevelDesigner {
	
	public static void main(String[] args) {
		String settingsFileName = null;
		if(args.length == 1) {
			if(args[0].charAt(0) == '-') {
				settingsFileName = args[0].substring(1, args[0].length());
			}
		}
		
		EditorWindow window = new EditorWindow(settingsFileName);
		window.setVisible(true);
	}
	
}
