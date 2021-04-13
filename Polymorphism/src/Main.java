public class Main {

	public static void main(String[] args) {
		
		Car OGcar = new Car("Generic Car", 6);
		Car car1 = new Camry("Camry", 4); // good polymorphism example
		Car car2 = new Accord(); // good polymorphism example
		Corolla car3 = new Corolla();
		
		System.out.println("The name of the car is: " + OGcar.getName());
		OGcar.accelerate();
		OGcar.brake();
		OGcar.startEngine(OGcar.getEngine());
		
		System.out.println("The name of the car is: " + car1.getName());
		car1.accelerate();
		car1.brake();
		car1.startEngine(car1.getEngine());
		
		System.out.println("The name of the car is: " + car2.getName());
		car2.accelerate();
		car2.brake();
		car2.startEngine(car2.getEngine());
		
		System.out.println("The name of the car is: " + car3.getName());
		car3.accelerate();
		car3.brake();
		car3.startEngine(car3.getEngine());
		
		printSpecs(OGcar);
		printSpecs(car3); // goes from being a Corolla object to Car object
		
	}
	
	//takes in a Car object also includes Camry, Corolla, and Accord objects showcases polymorphism and how an instance of a subclass can be of many types
	public static void printSpecs(Car car){
        System.out.println(car.getName() + " has " + car.getCylinder() + " cylinders");
	}
	
}
