/*
Session 2 HW: Exercise 2 : Vehicle Management System
Design a vehicle management system for a car rental company. the system needs to represent different types of vehicles (e.g., cars, trucks, and motorcycles) and their components (e.g., engines, wheels, and gps systems). your task is to determine which relationships should be modeled using inheritance ("is-a") and which should be modeled using composition ("has-a").

Requirements
1. Vehicles should have attributes like brand, model, and year.
2. Cars, trucks, and motorcycles should share common vehicle properties but may have additional specific features.
3. Engines, wheels, and gps systems are not vehicles but are essential parts of them.
4. The system should be designed for future extensibility (e.g., adding electric vehicles or self - driving cars).

Questions
1. which classes should inherit from a base class (is - a relationship) ?
2. which classes should be contained within other classes as components(has - a relationship) ?
3. how would you modify the design to support new vehicle types without major code changes ?


*/

// Specification file for Vehicle class - class declarations (classes, mbr vars, fn prototypes)

export module Vehicle;

import <iostream>;
import <string>;
import <memory>;
using namespace std;

// Fwd decl
class Engine;
class Wheels;
class GPSSystem;

// Base class
export class Vehicle {
    protected:
        string brand;
        string model;
        int year;
        unique_ptr<Engine> engine;
        unique_ptr <Wheels> wheels;
        unique_ptr <GPSSystem> gpsSystem;

    public:
        Vehicle(const string& b, const string& m, int y, unique_ptr<Engine> eng, unique_ptr <Wheels> wh, unique_ptr <GPSSystem> gps);  // cx
        virtual ~Vehicle() = default;   // dx

        // Getters
        string getBrand() const;
        string getModel() const;
        int getYear() const;
        const Engine& getEngine() const;
        const Wheels& getWheels() const;
        const GPSSystem& getGPSSystem() const;

        // Setters
        void setBrand(const string& b);
        void setModel(const string& m);
        void setYear(const int y);
        
        // Virtual fns (interface for derived classes)
        virtual void displayInfo() const;   // To display vehicle details
        virtual string getFuelType() const = 0;     // Pure virtual fn for fuel type (extensibility)
    };