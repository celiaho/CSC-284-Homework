import <iostream>;
import Vehicle;
import Engine;
import Wheels;
import GPSSystem;
import Car;
import Truck;
import Motorcycle;
using namespace std;

int main() {
	// Create component objects
	Engine engine1;				// Gas
	Engine engine2("Diesel");
	Engine engine3("Electric");
	Wheels wheels1;				// All-Season
	Wheels wheels2("Winter");
	Wheels wheels3("Summer");
	GPSSystem gpsSystem1;		// Tom Tom
	GPSSystem gpsSystem2("Rand McNally");
	GPSSystem gpsSystem3("Garmin");

	// Create vehicles
	Car car1("Toyota", "Camry", 1992, engine1, wheels1, gpsSystem1, 5);
	Truck truck1("Ram", "2500 Heavy Duty", 2004, engine2, wheels2, gpsSystem2, 3500.00f);
	// Cab configurations (e.g., Regular Cab, Crew Cab, Mega Cab).
	// Bed lengths.
	Motorcycle motorcycle1("Zero Motorcycles", "SR/F", 2025, engine3, wheels3, gpsSystem3, false);


	// Print vehicle inventory
	cout << "***** VEHICLE INVENTORY *****" << endl << endl;
	cout << "Car 1: " << endl << car1.toString() << endl << endl;
	cout << "Truck 1: " << endl << truck1.toString() << endl << endl;
	cout << "Motorcycle 1: " << endl << motorcycle1.toString() << endl << endl;

	return 0;
}