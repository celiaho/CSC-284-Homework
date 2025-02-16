export module Truck;

import <iostream>;
import <string>;
import <memory>;
using namespace std;

// Cx
Truck::Truck(const string& make, const string& model, const int& year, int numberOfDoors, int numberOfSeats, unique_ptr<Engine> engine, unique_ptr <Wheels> wheels, unique_ptr <GPSSystem> gpsSystem) : Vehicle(make, model, year, move(engine), move(wheels), move(gpsSystem)), numberOfDoors(numberOfDoors), numberOfSeats(numberOfSeats) {}

// Getters
int Truck::getNumberOfDoors() const { return numberOfDoors; }
int Truck::getNumberOfSeats() const { return numberOfSeats; }

// Setters
void Truck::setNumberOfDoors() const { return numberOfDoors; }
void Truck::setNumberOfSeats() const { return numberOfSeats; }

// Virtual fns
void Truck::getFuelType() const override {
	return "Diesel";
}

void Truck::displayInfo() const override {
	Vehicle::displayInfo();
	cout << "Number of Doors: " << numberOfDoors << endl;
	cout << "Number of Seats: " << numberOfSeats << endl;
}
