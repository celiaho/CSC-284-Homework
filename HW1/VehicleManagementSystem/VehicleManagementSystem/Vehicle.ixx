// Specification file - declarations (classes, mbr vars, fn prototypes)
export module Vehicle;

import Engine;
import Wheels;
import GPSSystem;
import <string>;
using namespace std;


// Base class
export class Vehicle {
protected:
    string brand;
    string model;
    int year;
    Engine engine;
    Wheels wheels;
    GPSSystem gpsSystem;

public:
    Vehicle() {};      // Default constructor
    Vehicle(const string& brand, const string& model, const int& year, Engine engine, Wheels wheels, GPSSystem gpsSystem);

    // Setters
    void setBrand(const string& brand);
    void setModel(const string& model);
    void setYear(const int& year);

    // Getters
    string getBrand() const;    // const after means it will only display, not change
    string getModel() const;
    int getYear() const;
    string getEngine() const;
    string getWheels() const;
    string getGPSSystem() const;

    // Virtual fns (interface for derived classes)
    virtual string toString() const;
};