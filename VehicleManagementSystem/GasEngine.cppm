module;

import Engine;
import <iostream>;
import <string>;
import <memory>;
using namespace std;

export class GasEngine : public Engine {
private:
	string fuelType;
	int cylinders;

public:
	GasEngine(int cyl);	// Cx declaration
	string getFuelType() const override;
	void displayInfo() const override;
};