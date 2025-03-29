/***
Notes from C++ Modules Tutorial in VS (2021) https://youtube.com/watch?v=qeW9sUu3adA
***/

//		.ixx for modules (imports, interfaces, implementations)

export module helloworldmodule;

// imports
import <iostream>;		// Replaces "#include <iostream>"
using namespace std;


/*** 
		INTERFACE - Declarations
*/

// public
export void HelloWorld();		// export = accessible outside of module

export struct HelloWorldStruct {
	int worldNum;
};

export class HelloWorldClass {
public:
	int worldNum;
};

namespace HelloWorldNS {
	export void Init() {
		cout << "Init" << endl;
	}

	void update() {
		cout << "Update" << endl;
	}

	export void Shutdown() {
		cout << "Shutdown" << endl;
	}
};

// private
void helloWorldImpl();			// No export = not accessible outside of module


/***
		IMPLEMENTATION 
*/

// public
void HelloWorld() {
	helloWorldImpl();			// generic interface
}

// private
void helloWorldImpl() {		// fn - implementation of generic interface
	cout << "Hello, World!" << endl;
	//std::cout << "Hello, World!" << std::endl;
	HelloWorldNS::update();
}