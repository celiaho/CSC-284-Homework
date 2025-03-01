export module Truck;

import Vehicle;
import Engine;
import Wheels;
import GPSSystem;
import <string>;
using namespace std;

export class Truck : public Vehicle {
private:
	float maxPayloadInPounds;	// Max weight truck can carry

public:
	Truck(const string& brand, const string& model, const int& year, Engine engine, Wheels wheels, GPSSystem gpsSystem, const float& maxPayloadInPounds);

	// Setters
	void setMaxPayloadInPounds(const float& maxPayloadInPounds);

	// Getters
	float getMaxPayloadInPounds() const;

	// Overridden virtual fns
	string toString() const override;
};