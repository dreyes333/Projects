
public class Couch {

	private String color;
	private String material;
	private String couchType;
	private double price;
	
	public Couch(String color, String material, String couchType, double price) {
		
		this.color = color;
		this.material = material;
		this.couchType = couchType;
		this.price = price;
		
	}
	
	public String toString() {
		return "Couch [color=" + color + ", material=" + material + ", couchType=" + couchType + ", price=" + price
				+ "]\n";
	}

	public String getColor() {
		return color;
	}
	public void setColor(String color) {
		this.color = color;
	}
	public String getMaterial() {
		return material;
	}
	public void setMaterial(String material) {
		this.material = material;
	}

	public String getCouchType() {
		return couchType;
	}

	public void setCouchType(String couchType) {
		this.couchType = couchType;
	}

	public double getPrice() {
		return price;
	}

	public void setPrice(double price) {
		this.price = price;
	}
	
	
	
}
