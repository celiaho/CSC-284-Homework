export module Car;

import Vehicle;
import <iostream>;
import <string>;
import <memory>;
using namespace std;

export class Car : public Vehicle {
private:
	int numberOfDoors;
	int numberOfSeats;

public:
	// Cx
	Car(const string& b, const string& m, int y, int doors, int seats, unique_ptr<Engine> eng, unique_ptr <Wheels> wh, unique_ptr <GPSSystem> gps);

	// Implementations of pure virtual fns (declarations only)
	string getFuelType() const override;
	void displayInfo() const override;

	// Getters
	int getNumberOfDoors() const;
	int getNumberOfSeats() const;

	// Setters
	void setNumberOfDoors(int doors);
	void setNumberOfSeats(int seats);
};

