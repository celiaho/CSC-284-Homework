//implementation file - mbr fns

export module Vehicle;

import <iostream>;
import <string>;
import <memory>;
import Engine;
import Wheels;
import GPSSystem;
using namespace std;

// Cx
Vehicle::Vehicle(const string& b, const string& m, int y, unique_ptr<Engine> eng, unique_ptr<Wheels> wh, unique_ptr<GPSSystem> gps) : brand(b), model(m), year(y), engine(move(eng)), wheels(move(wh)), gpsSystem(move(gps)) {}

// Getters
string Vehicle::getBrand() const { return brand; }
string Vehicle::getModel() const { return model; }
 int Vehicle::getYear()const { return year; }
const Engine& Vehicle::getEngine() const { return *engine; }
const Wheels& Vehicle::getWheels() const { return *wheels; }
const GPSSystem& Vehicle::getGPSSystem() const { return *gpsSystem; }

// Setters
void Vehicle::setBrand(const string& b) { brand = b; }
void Vehicle::setModel(const string& m) { model = m; }
void Vehicle::setYear(const int& y) { year = y; }

// Virtual fns (interface for derived classes)
void Vehicle::displayInfo() const {
    cout << "Brand: " << brand << ", Model: " << model << ", Year: " << year << endl;
    cout << "Fuel Type: " << getFuelType() << endl;
    engine->displayInfo();
    wheels->displayInfo();
    gpsSystem->displayInfo();
}

// Dx def not needed bc "= default" used in .h