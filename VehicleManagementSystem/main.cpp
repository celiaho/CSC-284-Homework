import <iostream>;
import Vehicle;
import Car;
import Truck;
import Motorcycle;
using namespace std;

int main() {
	// Create an Engine
	auto gasEngine = make_unique<GasEngine>("V8");

	// Create Wheel
	auto wheels = make_unique<Wheels>(4);

	// Create a GPSSystem
	auto gpsSystem = make_unique<GPSSystem>("Garmin");

	// Create a Car object
	Car car1("Toyota", "Camry", "2023", 4, 5, move(engine), move(wheels), move(gpsSystem));

	// Demo Car object
	car1.displayInfo();

	return 0;


	

}