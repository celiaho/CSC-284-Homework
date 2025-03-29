/***
Notes from C++ Modules Tutorial in VS (2021) https://youtube.com/watch?v=qeW9sUu3adA
***/

//		.cppm for global scope code e.g. main

import helloworldmodule;		// Import custom module (no <>)
import <cstdlib>;				// For "system("pause")"
using namespace std;


int main() {
	HelloWorld();

	HelloWorldStruct my_struct{ 1 };
	HelloWorldClass my_class{ 2 };

	HelloWorldNS::Init();
	HelloWorldNS::Shutdown();

	system("pause");
	//return 0;
}