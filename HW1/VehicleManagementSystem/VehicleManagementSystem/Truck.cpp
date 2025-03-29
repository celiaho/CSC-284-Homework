import Truck;

import <string>;
import <iomanip>;	// for formatting e.g. fixed and setPrecision
import <sstream>;	// for stringstream, which allows I/O ops on strings incl. formatting data in a string
using namespace std;
using std::setprecision;

// Cx
Truck::Truck(const string& brand, const string& model, const int& year, Engine engine, Wheels wheels, GPSSystem gpsSystem, const float& maxPayloadInPounds) : Vehicle(brand, model, year, engine, wheels, gpsSystem), maxPayloadInPounds(maxPayloadInPounds) {}

// Setters
void Truck::setMaxPayloadInPounds(const float& maxPayloadInPounds) {
	this->maxPayloadInPounds = maxPayloadInPounds;
}

// Getters
float Truck::getMaxPayloadInPounds() const {
	return maxPayloadInPounds;
}

// Overridden virtual fns
string Truck::toString() const {
	//stringstream stringstream;	// Create a stringstream
	/////// ??? Why does line below cause error "'setPrecision': identifier not found"?
	//stringstream << fixed << setPrecision(2) << maxPayloadInPounds;	// Format to 2 decimal places
	

	return Vehicle::toString() +
		",\n \tMax Payload in Pounds: " + to_string(maxPayloadInPounds);
		//Line below doesn't work because of setPrecision error in line 26
		//",\n \tMax Payload in Pounds: " + stringstream.str();	// Get the formatted string
}
