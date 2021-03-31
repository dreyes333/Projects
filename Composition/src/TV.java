
public class TV {
	
	private double width;
	private String brand;
	private String color;
	private boolean is4k;
	private double price;
	
	public TV(double width, String brand, String color, boolean is4k, double price) {
		this.width = width;
		this.brand = brand;
		this.color = color;
		this.is4k = is4k;
		this.price = price;
	}

	
	public String toString() {
		return "TV [width=" + width + ", brand=" + brand + ", color=" + color + ", is4k=" + is4k + ", price=" + price
				+ "]\n";
	}

	public double getWidth() {
		return width;
	}

	public void setWidth(double width) {
		this.width = width;
	}

	public String getBrand() {
		return brand;
	}

	public void setBrand(String brand) {
		this.brand = brand;
	}

	public String getColor() {
		return color;
	}

	public void setColor(String color) {
		this.color = color;
	}

	public boolean isIs4k() {
		return is4k;
	}

	public void setIs4k(boolean is4k) {
		this.is4k = is4k;
	}

	public double getPrice() {
		return price;
	}

	public void setPrice(double price) {
		this.price = price;
	}

}
