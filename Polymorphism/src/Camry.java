
public class Camry extends Car {
	
	public Camry(String name, int cylinder) {
		super(name, cylinder);
	}
	
	public void brake() {
		System.out.println("The camry is braking!!!");
	}
	
	public void accelerate() {
		System.out.println("The camry is accelerating!!!!!!!!!!");
	}
	
	public void startEngine(boolean engine) {
		if(engine == true) {
			System.out.println("Camry engine is already on!");
		} else {
			engine = true;
			System.out.println("Camry engine has been turned on!");
		}
	}
}
