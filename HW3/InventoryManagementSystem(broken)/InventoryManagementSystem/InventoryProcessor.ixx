export module InventoryProcessorModule;

import <vector>;
import <string>;
import <"C:\BHCC\2025_Spring\CSC-284_Advanced_CPP\CSC-284-Homework\InventoryManagementSystem\InventoryManagementSystem\json.hpp">;
using namespace std;

export template <typename T>
class InventoryProcessor {
public:
	vector<T> readFromFile(const string& filename);
	void process(const vector<T>& data);
	void writeToFile(const string& filename);
};