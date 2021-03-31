
public class AudioDevice {

	private String deviceType;
	private String color;
	private String brand;
	private int numPieces;
	private double price;
	
	public AudioDevice(String deviceType, String color, String brand, int numPieces, double price) {
		this.deviceType = deviceType;
		this.color = color;
		this.brand = brand;
		this.numPieces = numPieces;
		this.price = price;
	}
	
	public String toString() {
		return "AudioDevice [deviceType=" + deviceType + ", color=" + color + ", brand=" + brand + ", numPieces="
				+ numPieces + ", price=" + price + "]\n";
	}

	public String getDeviceType() {
		return deviceType;
	}
	
	public void setDeviceType(String deviceType) {
		this.deviceType = deviceType;
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
	public int getNumPieces() {
		return numPieces;
	}
	public void setNumPieces(int numPieces) {
		this.numPieces = numPieces;
	}
	public double getPrice() {
		return price;
	}
	public void setPrice(double price) {
		this.price = price;
	}
	
	
	
	
}
