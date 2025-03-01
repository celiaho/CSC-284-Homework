import Motorcycle;

import <string>;
using namespace std;

// Cx
Motorcycle::Motorcycle(const string& brand, const string& model, const int& year, Engine engine, Wheels wheels, GPSSystem gpsSystem, const bool& hasSidecar) : Vehicle(brand, model, year, engine, wheels, gpsSystem), hasSidecar(hasSidecar) {}

// Setters
void Motorcycle::setHasSidecar(const bool& hasSideCar) { 
	this->hasSidecar = hasSidecar;
}

// Getters
bool Motorcycle::getHasSidecar() const { 
	return hasSidecar; 
}

// Overridden virtual fns
string Motorcycle::toString() const {
	return Vehicle::toString() +
		",\n \tHas Sidecar: " + (hasSidecar ? "Yes" : "No");
}