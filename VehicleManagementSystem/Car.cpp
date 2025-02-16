export module Car;

import <iostream>;
import <string>;
import <memory>;
using namespace std;

// Cx
Car::Car(const string& make, const string& model, const int& year, int numberOfDoors, int numberOfSeats, unique_ptr<Engine> engine, unique_ptr <Wheels> wheels, unique_ptr <GPSSystem> gpsSystem) : Vehicle(make, model, year, move(engine), move(wheels), move(gpsSystem)), numberOfDoors(numberOfDoors), numberOfSeats(numberOfSeats) {}

// Getters
int Car::getNumberOfDoors() const { return numberOfDoors; }
int Car::getNumberOfSeats() const { return numberOfSeats; }

// Setters
void Car::setNumberOfDoors() const { return numberOfDoors; }
void Car::setNumberOfSeats() const { return numberOfSeats; }

// Virtual fns
void Car::getFuelType() const override {
	return "Gasoline";
}

void Car::displayInfo() const override {
	Vehicle::displayInfo();
	cout << "Number of Doors: " << numberOfDoors << endl;
	cout << "Number of Seats: " << numberOfSeats << endl;
}
