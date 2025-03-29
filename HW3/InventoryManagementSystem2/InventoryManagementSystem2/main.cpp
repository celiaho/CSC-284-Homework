import InventoryProcessor_Product2;
import Product;
import <iostream>;
import <string>;
import <filesystem>;
import <stdexcept>;
using namespace std;

int main() {
	string inputFileName;
	string outputFileName;
	bool inputFileExists = false;	// Program starts with no input file defined

	while (!inputFileExists) {
		// Prompt user for input file name
		cout << "Enter the name of the inventory JSON file you want to read (e.g., inventory.json): ";
		cin >> inputFileName;

		// Check that requested input file exists
		filesystem::path inputPath(inputFileName);
		if (filesystem::exists(inputPath)) {
			inputFileExists = true;
		}
		else {
			cerr << "Error: Input file \"" << inputFileName << "\" does not exist." << endl;
			cout << "Please try again." << endl;
		}
	}

	// Prompt user for output file name (after input file name is validated)
	cout << "Thank you. I will read this file, process the data, and write results to an output summary JSON file. What would you like to name this file (e.g., summary.json)? ";
	cin >> outputFileName;

	InventoryProcessor<Product> inventoryProcessor;	// Instantiate specialization module

	// Read, process, write results
	try {
		inventoryProcessor.readFromFile(inputFileName);
		inventoryProcessor.process();
		inventoryProcessor.writeToFile(outputFileName);

		cout << "Inventory processed successfully. Summary written to " << outputFileName << "." << endl;
	}
	catch (const runtime_error& e) {
		cerr << "Runtime Error: " << e.what() << endl;
		return 1;
	}
	catch (const exception& e) {
		cerr << "An unexpected error occurred: " << e.what() << endl;
		return 1;
	}

	return 0;
}