// Implementation file - definitions
import Vehicle;

import <iostream>;		// for cout
import <string>;
using namespace std;


// Cx
Vehicle::Vehicle(const string& brand, const string& model, const int& year, Engine engine, Wheels wheels, GPSSystem gpsSystem) {
	this->brand = brand;
	this->model = model;
	this->year = year;
	this->engine = engine;
	this->wheels = wheels;
	this->gpsSystem = gpsSystem;
}

// Setters
void Vehicle::setBrand(const string& brand) { this->brand = brand; }
void Vehicle::setModel(const string& model) { this->model = model; }
void Vehicle::setYear(const int& year) { this->year = year; }

// Getters
string Vehicle::getBrand() const { return brand; }
string Vehicle::getModel() const { return model; }
int Vehicle::getYear() const { return year; }
string Vehicle::getEngine() const { return engine.getFuelType(); }
string Vehicle::getWheels() const { return wheels.getSeasonType(); }
string Vehicle::getGPSSystem() const { return gpsSystem.getBrand(); }

// Virtual fns (interface for derived classes)
string Vehicle::toString() const {
	return "\tBrand: " + brand +
		",\n \tModel: " + model +
		",\n \tYear: " + to_string(year) +  // to_string converts int to string
		",\n \tEngine: " + engine.getFuelType() +
		",\n \tWheels: " + wheels.getSeasonType() +
		",\n \tGPS System: " + gpsSystem.getBrand();
}