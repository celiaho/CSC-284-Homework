export module Motorcycle;

import Vehicle;
import Engine;
import Wheels;
import GPSSystem;
import <string>;
using namespace std;

export class Motorcycle : public Vehicle {
private:
	bool hasSidecar;

public:
	// Cx
	Motorcycle(const string& brand, const string& model, const int& year, Engine engine, Wheels wheels, GPSSystem gpsSystem, const bool& hasSidecar);

	// Setters
	void setHasSidecar(const bool& hasSidecar);

	// Getters
	bool getHasSidecar() const;

	// Overridden virtual fns
	string toString() const override;
};

