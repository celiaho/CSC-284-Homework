export module Motorcycle;

import Vehicle;
import <iostream>;
import <string>;
import <memory>;
using namespace std;

export class Motorcycle : public Vehicle {
private:
	bool hasSidecar;
	int numberOfSeats;

public:
	// Cx
	Motorcycle(const string& b, const string& m, int y, boolean sidecar, int seats, unique_ptr<Engine> eng, unique_ptr <Wheels> wh, unique_ptr <GPSSystem> gps);

	// Implementations of pure virtual fns (declarations only)
	string getFuelType() const override;
	void displayInfo() const override;

	// Getters
	int getNumberOfSeats() const;
	boolean getHasSidecar() const;

	// Setters
	void setHasSidecar(boolean sidecar);
	void setNumberOfSeats(int seats);
};

