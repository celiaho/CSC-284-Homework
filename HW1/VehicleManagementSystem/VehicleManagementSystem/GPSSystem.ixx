export module GPSSystem;

import <string>;
using namespace std;

export class GPSSystem {
private:
	string brand;

public:
	GPSSystem() : GPSSystem("Tom Tom") {};

	GPSSystem(string brand) {
		this->brand = brand;
	}

	string getBrand() const {
		return brand;
	}
};