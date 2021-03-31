
public class Main {
	
	public static void main(String[] args) {
		
		//create all objects in the room
		TV tv = new TV(60.0, "samsung", "black", true, 800.0 );
		MediaConsole console = new MediaConsole(60, 36, "brown", 300.00);
		Couch couch = new Couch("brown", "leather", "recliner", 1000.00);
		AudioDevice device = new AudioDevice("surround sound", "black", "samsung", 5, 1000.00);
		Fridge fridge = new Fridge("black", "LG", 200.00, true, true);
		//create room
		EntertainmentRoom entRoom = new EntertainmentRoom("brown", tv, console, couch, device, fridge);

		entRoom.getFridge().toString(); //since the getter for fridge is not private you can access is it in this class and access that objects methods directly
		
		entRoom.roomDetails();
		
	}
	
}
