
public class Fridge {
	
	private String color;
	private String brand;
	private double price;
	private boolean isMini;
	private boolean hasCooler;
	
	public Fridge(String color, String brand, double price, boolean isMini, boolean hasCooler) {
		this.setColor(color);
		this.setBrand(brand);
		this.setPrice(price);
		this.setMini(isMini);
		this.setHasCooler(hasCooler);
	}
	
	public String toString() {
		return "Fridge [color=" + color + ", brand=" + brand + ", price=" + price + ", isMini=" + isMini
				+ ", hasCooler=" + hasCooler + "]\n";
	}

	public String getColor() {
		return color;
	}

	public void setColor(String color) {
		this.color = color;
	}

	public String getBrand() {
		return brand;
	}

	public void setBrand(String brand) {
		this.brand = brand;
	}

	public double getPrice() {
		return price;
	}

	public void setPrice(double price) {
		this.price = price;
	}

	public boolean isMini() {
		return isMini;
	}

	public void setMini(boolean isMini) {
		this.isMini = isMini;
	}

	public boolean isHasCooler() {
		return hasCooler;
	}

	public void setHasCooler(boolean hasCooler) {
		this.hasCooler = hasCooler;
	}
	
	
	
	
	

}
