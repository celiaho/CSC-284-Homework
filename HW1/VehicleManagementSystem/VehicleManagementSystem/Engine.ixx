export module Engine;

import <string>;
using namespace std;


export class Engine {
private:
	string fuelType;

public:
	Engine() : Engine("Gas") {};		// Direct mbr initializn: When default cx is called, fuelType is set to gas by default

	Engine(string fuelType) {
		this->fuelType = fuelType;
	}
	// Engine(string fuelType) : fuelType(fuelType) {}  // Same as line above

	string getFuelType() const {
		return fuelType;
	}

	/*string toString() const {
		return fuelType;
	}*/
};