/*
Inventory Management System Using Templates and JSON
Problem Overview:
You have been hired to develop a generic inventory management system that processes inventory data stored in a JSON file. The system should:
Read inventory data from a JSON file, which contains a list of products with attributes like name, category, quantity, and price.
Use a templated class to process inventory items generically, allowing operations on different types of product attributes (e.g., int, double, std::string).
Perform operations based on data type:
Compute the total stock value (quantity × price).
Find the most expensive product.
Generate a summary report categorized by product type.
Write the processed results back to a JSON file for further analysis.
*/

import <fstream>;	// for reading/writing to files
import <iostream>;
import <string>;
using namespace std;

int main() {
	// Read the file
	ifstream inputFile{ "inventory.json" };		// Create an input file stream
	string line;
	if (inputFile.is_open()) {
		while (getline(inputFile, line)) {
			cout << line << endl;
		}
		inputFile.close();
	} else {
		cerr << "Unable to open input file." << endl;
	}

	// Print to file
	// Put it to objects
	// Make summary ... _put

	return 0;
}