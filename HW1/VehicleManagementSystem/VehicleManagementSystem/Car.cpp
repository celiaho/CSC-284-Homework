import Car;

import <string>;
using namespace std;

// Cx
Car::Car(const string& brand, const string& model, const int& year, Engine engine, Wheels wheels, GPSSystem gpsSystem, const int& numberOfSeats) : Vehicle(brand, model, year, engine, wheels, gpsSystem), numberOfSeats(numberOfSeats) {}

// Setters
void Car::setNumberOfSeats(const int& numberOfSeeats) { 
	this->numberOfSeats = numberOfSeats; 
}

// Getters
int Car::getNumberOfSeats() const { 
	return numberOfSeats; 
}

// Overridden virtual fns
string Car::toString() const {
	return Vehicle::toString() +
		",\n \tNumber of Seats: " + to_string(numberOfSeats);
}