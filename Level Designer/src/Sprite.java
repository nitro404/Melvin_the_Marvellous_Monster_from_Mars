import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class Sprite {
	
	private File imageFile;
	private BufferedImage image;
	
	public Sprite(String fileName, String spriteDirectory) {
		String imagePath = spriteDirectory;
		if(imagePath.charAt(imagePath.length() - 1) != '\\' && imagePath.charAt(imagePath.length() - 1) != '/') {
			imagePath += '\\';
		}
		imagePath += fileName;
		
		this.imageFile = new File(imagePath);
		
		if(!this.imageFile.exists() || !this.imageFile.isFile()) {
			System.out.println("ERROR: Sprite: \"" + this.imageFile.getPath() + "\" does not exist or is an invalid image.");
			System.exit(1);
		}
		
		try {
			this.image = ImageIO.read(this.imageFile);
		}
		catch(IOException e) {
			System.out.println("ERROR: Unable to read sprite image: \"" + this.imageFile.getPath() + "\".");
			System.exit(1);
		}
	}
	
	public Sprite(String fileName, String spriteDirectory, int x, int y, int width, int height) {
		String imagePath = spriteDirectory;
		if(imagePath.charAt(imagePath.length() - 1) != '\\' && imagePath.charAt(imagePath.length() - 1) != '/') {
			imagePath += '\\';
		}
		imagePath += fileName;
		
		this.imageFile = new File(imagePath);
		
		if(!this.imageFile.exists() || !this.imageFile.isFile()) {
			System.out.println("ERROR: Sprite: \"" + this.imageFile.getPath() + "\" does not exist or is an invalid image.");
			System.exit(1);
		}
		
		try {
			this.image = ImageIO.read(this.imageFile).getSubimage(x, y, width, height);
		}
		catch(IOException e) {
			System.out.println("ERROR: Unable to read sprite image: \"" + this.imageFile.getPath() + "\".");
			System.exit(1);
		}
	}
	
	public Sprite(BufferedImage image) {
		if(image == null) {
			System.out.println("ERROR: Sprite cannot be created from null image.");
			System.exit(1);
		}
		
		this.image = image;
	}
	
	public Sprite(BufferedImage image, int x, int y, int width, int height) {
		if(image == null) {
			System.out.println("ERROR: Sprite cannot be created from null image.");
			System.exit(1);
		}
		
		this.image = image.getSubimage(x, y, width, height);
	}
	
	public int getWidth() { return this.image.getWidth(); }
	public int getHeight() { return this.image.getHeight(); }
	
	public BufferedImage getImage() { return this.image; }
	
	public void paintOn(Graphics g, int x, int y) {
		((Graphics2D) g).drawImage(image, null, x, y);
	}
	
	public void paintOnCentered(Graphics g, int x, int y) {
		((Graphics2D) g).drawImage(image, null, (int) (x - (image.getWidth() / 2.0)), (int) (y - (image.getHeight() / 2.0)));
	}
	
}
