export module Engine;

import <iostream>;
import <string>;
import <memory>;
using namespace std;

export class Engine {	// Abstract base class
public:
	virtual string getFuelType() const = 0;
	virtual void displayInfo() const;
	virtual ~Engine() = default;	// Virtual dx
};