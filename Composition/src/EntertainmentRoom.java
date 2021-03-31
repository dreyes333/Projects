
public class EntertainmentRoom {
	
	private String floorColor;
	private TV tv;
	private MediaConsole console;
	private Couch couch;
	private AudioDevice audio;
	private Fridge  fridge;
	
	public EntertainmentRoom(String floorColor, TV tv, MediaConsole console, Couch couch, AudioDevice audio, Fridge fridge) {
		this.setFloorColor(floorColor);
		this.setTv(tv);
		this.setConsole(console);
		this.setCouch(couch);
		this.setAudio(audio);
		this.setFridge(fridge);
	}
	
	//shows you a list of all objects in the room by calling toString() for each one and then calls private method that calculates total of all objects in the room, dont need any of the getters tbh because they cant be acccessed in another class except for fridge
	public void roomDetails() {
		double total;
		System.out.println("The entertainment room consists of these items and their specs: \n");
		System.out.print(tv.toString());
		System.out.print(console.toString());
		System.out.print(couch.toString());
		System.out.print(audio.toString());
		System.out.println(fridge.toString());
		total = totalCost();
		System.out.println("The total cost of this extravagant entertainment room is: $" + total);
		
	}
	
	//gets a sum of the prices of all products in the room
	private double totalCost() {
		return tv.getPrice() + console.getPrice() + couch.getPrice() + audio.getPrice() + getFridge().getPrice(); // can also do fridge.getPrice()
	}
	
	public String getFloorColor() {
		return floorColor;
	}

	public void setFloorColor(String floorColor) {
		this.floorColor = floorColor;
	}

	private TV getTv() {
		return tv;
	}

	public void setTv(TV tv) {
		this.tv = tv;
	}

	private MediaConsole getConsole() {
		return console;
	}

	public void setConsole(MediaConsole console) {
		this.console = console;
	}

	private Couch getCouch() {
		return couch;
	}

	public void setCouch(Couch couch) {
		this.couch = couch;
	}

	private AudioDevice getAudio() {
		return audio;
	}

	public void setAudio(AudioDevice audio) {
		this.audio = audio;
	}

	public Fridge getFridge() {
		return fridge;
	}

	public void setFridge(Fridge fridge) {
		this.fridge = fridge;
	}
	
	

}
