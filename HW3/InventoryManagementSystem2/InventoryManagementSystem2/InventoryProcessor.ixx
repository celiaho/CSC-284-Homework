export module InventoryProcessor;

import <fstream>;	// for reading/writing to files
import <iostream>;
import <string>;
import <sstream>;
import <stdexcept>;	// for runtime_error
import <vector>;
import <json.hpp>;
using namespace std;

export template <typename T>
class InventoryProcessor {
protected:
	vector<T> inventoryData;	// Vector to store & process inventory data objects of type T

public:
	InventoryProcessor() {}	// Cx

	vector<T> readFromFile(const string& filename) {	// Read JSON data into a vector of T objects.
		ifstream inputFile{ filename };	// Create an input file stream & read from beginning (", ios_base::in" not appended bc it's the default mode)
		if (!inputFile.is_open()) {
			cerr << "Error while opening input file " << filename << "." << endl;
			throw runtime_error("Could not open input file " + filename + ".");
		}
		nlohmann::json j;	// Create a JSON object to store the parsed data
		inputFile >> j;		// Read all JSON content from input file stream into the JSON object 

		vector<T> data;		// Create an empty vector to store the objects of type T

		// Type-specific parsing logic goes in the specialization

		return data;	// Return the vector of T objects
	}

	void process() {	// Do data analysis & generate useful results (e.g., sum, average or summaries depending on type).
		cerr << "Warning: Generic process() does nothing. Implement in specialization." << endl;
	}

	void writeToFile(const string& filename) {	// Write results to a JSON file.
		ofstream outputFile{ filename };
		if (!outputFile.is_open()) {
			cerr << "Error while opening output file " << filename << "." << endl;
			throw runtime_error("Could not open output file " + filename + ".");
		}

		nlohmann::json outputJson;					// Create an empty JSON object
		// Write the empty JSON object (for the generic case) to the file with indentation and floating-point precision
		stringstream ss;
		ss << fixed << setprecision(2) << outputJson.dump(4) << endl;
		outputFile << ss.str();
	}
};