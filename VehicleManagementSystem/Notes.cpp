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
Notes
- Files needed :
-.h files(Headers) : Declare your classes, member variables, and function prototypes.Include only the declarations, not the implementations.
- .cpp files(Sources) : Define the actual implementationsf o your classes' functions.  Include the corresponding .h file at the top of your .cpp file.

- Vehicle.h w / class dec(mbr vars, cx, getters & setters)
- include Engine.h w / headers for component classes, wheels.h, gpyssystem.h
sssssssssssssssssss
- Vehicle.cpp w / mbr fns(displayInfo()


    - Make model year should be protected pure virtual functions(interfaces).Maybe include getFuelType() or displayInfo() for vehicle details ? (class Engine {
    public: virtual std::string getFuelType() const = 0; // extensibility) 
          virtual ~Engine() = "" / default;
})
- Engines wheels etc should be private

    - Aggregation - Car rental company has Vehicles(abstract base class)
    - Cars trucks moto extend Vehicle(derived classes).Later can add ElectricCar, SelfDrivingCar.Cars, trucks, motorcycles are Vehicles
    - All vehicles have engines, wheels, gps(has - a relationships).Interfaces or abstract base classes ? E.g. class gasEngine :public Engine {
    string getFuelType const override{ return "Gasoline"; } vs. class ElectricMotor :public Engine { ...return "Electricity"; }

  

        - Derived classes will have other features like number of doors, cargo capacity, sidecar attachment

            Vehicle::Vehicle() {
            make = "";
            model = "";
            year = "";
        }

        Vehicle::Vehicle(string make) {
            this.make = make;
            model = "";
            year = "";
        }

