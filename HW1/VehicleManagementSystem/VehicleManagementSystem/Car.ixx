export module Car;

import Vehicle;
import Engine;
import Wheels;
import GPSSystem;
import <string>;
using namespace std;

export class Car : public Vehicle {
private:
	int numberOfSeats;

public:
	// Cx
	Car(const string& brand, const string& model, const int& year, Engine engine, Wheels wheels, GPSSystem gpsSystem, const int& numberOfSeats);

	// Setters
	void setNumberOfSeats(const int& numberOfSeats);

	// Getters
	int getNumberOfSeats() const;


	// Overridden virtual fns
	string toString() const override;
};