
public class MediaConsole {
	
	private double width;
	private double height;
	private String color;
	private double price;
	
	public MediaConsole(double width, double height, String color, double price) {
		this.width = width;
		this.height = height;
		this.color = color;
		this.price = price;
	}
	
	public String toString() {
		return "MediaConsole [width=" + width + ", height=" + height + ", color=" + color + ", price=" + price + "]\n";
	}

	public double getWidth() {
		return width;
	}
	public void setWidth(double width) {
		this.width = width;
	}
	public double getHeight() {
		return height;
	}
	public void setHeight(double height) {
		this.height = height;
	}
	public String getColor() {
		return color;
	}
	public void setColor(String color) {
		this.color = color;
	}

	public double getPrice() {
		return price;
	}

	public void setPrice(double price) {
		this.price = price;
	}
	
	
	
}
