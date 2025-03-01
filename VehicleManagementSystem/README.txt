Session 2 HW: Exercise 2 : Vehicle Management System

INSTRUCTIONS
Design a vehicle management system for a car rental company. the system needs to represent different types of vehicles (e.g., cars, trucks, and motorcycles) and their components (e.g., engines, wheels, and GPS systems). Your task is to determine which relationships should be modeled using inheritance ("is-a") and which should be modeled using composition ("has-a").

Requirements
1. Vehicles should have attributes like brand, model, and year.
2. Cars, trucks, and motorcycles should share common vehicle properties but may have additional specific features.
3. Engines, wheels, and GPS systems are not vehicles but are essential parts of them.
4. The system should be designed for future extensibility (e.g., adding electric vehicles or self-driving cars).


QUESTIONS
1. Which classes should inherit from a base class (is-a relationship)?
	Car, Truck, and Motorcycle classes should inherit from the Vehicle base class.

2. Which classes should be contained within other classes as components (has-a relationship)? 
	Engine, Wheels, and GPSSystem classes should be contained within Vehicle, Car, Truck, and Motorcycle classes as components. 

3. How would you modify the design to support new vehicle types without major code changes?
	Current design allows for new derived classes (e.g. "ElectricCar" or "Plane") to be added along with any unique attributes they have. If new vehicle types are based on fuel type (e.g. "ElectricCar" or "ElectricTruck" or "HybridCar"), it may be redundant to have fuelType as an Engine attribute.


MY QUESTIONS FOR PROFESSOR
1. Why does using setPrecision to format MaxPayloadInPounds in the Truck.cpp file cause an error? Error details in code comments.