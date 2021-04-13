public class Car {
	private boolean engine;
	private int cylinder;
	private int numWheels;
	private String name;
	
	public Car(String name, int cylinder) {
		this.name = name;
		this.cylinder = cylinder;
		engine = true;
		numWheels = 4;
	}
	
	public void startEngine(boolean engine) {
		if(engine == true) {
			System.out.println("Car engine is already on!");
		} else {
			engine = true;
			System.out.println("Car engine has been turned on!");
		}
	}
	
	public void brake() {
		System.out.println("The car is braking!!");
	}
	
	public void accelerate() {
		System.out.println("The car is now accelerating!!!!!!!!!");
	}
	
	public boolean getEngine() {
		return engine;
	}
	
	public int getCylinder() {
		return cylinder;
	}
	
	public int getNumWheels() {
		return numWheels;
	}
	
	public String getName() {
		return name;
	}
	
}