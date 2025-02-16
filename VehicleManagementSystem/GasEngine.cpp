module GasEngine;

import <iostream>;
import <string>;
import <memory>;
using namespace std;

GasEngine::GasEngine(int cyl) : cylinders(cyl), fuelType("Gasoline") {}

string GasEngine::getFuelType() const override {
	return fuelType;
}

void GasEngine::displayInfo() const override {
	cout << "Gas Engine - " << cylinders << " cylinders" << endl;
}