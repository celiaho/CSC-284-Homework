export module Motorcycle;

import <iostream>;
import <string>;
import <memory>;
using namespace std;

// Cx
Motorcycle::Motorcycle(const string& make, const string& model, const int& year, bool hasSidecar, int numberOfSeats, unique_ptr<Engine> engine, unique_ptr <Wheels> wheels, unique_ptr <GPSSystem> gpsSystem) : Vehicle(make, model, year, move(engine), move(wheels), move(gpsSystem)), hasSidecar(hasSidecar), numberOfSeats(numberOfSeats) {}

// Getters
boolean Motorcycle::getHasSidecar() const { return hasSidecar; }
int Motorcycle::getNumberOfSeats() const { return numberOfSeats; }

// Setters
void Motorcycle::setHasSidecar() const { return hasSidecar; }
void Motorcycle::setNumberOfSeats() const { return numberOfSeats; }

// Virtual fns
void Motorcycle::getFuelType() const override {
	return "Gasoline";
}

void Motorcycle::displayInfo() const override {
	Vehicle::displayInfo()
	cout << "Has Sidecar: " << hasSidecar << endl;
	cout << "Number of Seats: " << numberOfSeats << endl;
}
