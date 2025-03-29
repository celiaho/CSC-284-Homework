#include <json.hpp>	// Placing this here with & without ; yields these errors

export module InventoryProcessor_Product2;

import Product;
import InventoryProcessor;

import <fstream>;
import <iostream>;
import <string>;
import <sstream>;
import <stdexcept>;
import <vector>;
import <map>;
import <iomanip>;

//import <json.hpp>;	// Import suddenly stopped working; I tried half a dozen things and reverting to #include is the only thing that erases the errors.

using namespace std;

export template <>	// Syntax for specialization
class InventoryProcessor<Product> {
protected:
	vector<Product> inventoryData;

private:
	// Create member variables for process()
	double totalStockValue = 0.0;	// The value of all items in inventory
	Product mostExpensiveProduct;
	map<string, int> totalItemsInCategory;
	map<string, double> totalValueOfCategory;	// The value of all items in the category

public:
	InventoryProcessor() = default;

	// Read JSON data into a vector of T objects.
	vector<Product> readFromFile(const string& filename) {
		ifstream inputFile{ filename };	// Create an input file stream & read from beginning (", ios_base::in" not appended bc it's the default mode)
		if (!inputFile.is_open()) {
			cerr << "Error while opening input file " << filename << "." << endl;
			throw runtime_error("Could not open input file " + filename + ".");
		}

		// Parse JSON: Create a nlohmann::json object and read the contents of the file into it.
		nlohmann::json j;	// Create a JSON object to store the parsed data
		inputFile >> j;		// Read all JSON content from input file stream into the JSON object 

		inventoryData.clear();

		// Check for the "inventory" array
		if (j.contains("inventory") && j["inventory"].is_array()) {
			for (const auto& item : j["inventory"]) {	// Iterate through product items
				// Check if it's a valid product object
				if (item.is_object() &&
					item.contains("name") && item["name"].is_string() &&
					item.contains("category") && item["category"].is_string() &&
					item.contains("quantity") && item["quantity"].is_number_integer() &&
					item.contains("price") && item["price"].is_number_float()
					) {

					// Create & add Product to vector
					inventoryData.emplace_back(
						item["name"].get<string>(),
						item["category"].get<string>(),
						item["quantity"].get<int>(),
						item["price"].get<double>()
					);
				}
				else {
					cerr << "Warning: Skipping invalid product entry in JSON." << endl;
				}
			}
		}
		else {
			cerr << "Warning: JSON file does not have the expected \"inventory\" array." << endl;
		}

		return inventoryData;	// Return the filled vector
	}

	// Do data analysis to generate useful results (sum, average or summaries depending on type).
	void process() {
		if (inventoryData.empty()) {
			cerr << "Warning: No inventory data to process." << endl;
			// Reset all calculations for the empty inventory
			totalStockValue = 0.0;
			mostExpensiveProduct = Product("", "", 0, 0.0);
			totalItemsInCategory.clear();
			totalValueOfCategory.clear();
			return;
		}

		// Initialize/reset all calculations for the non-empty inventory
		totalStockValue = 0.0;
		double highestProductPrice = -1.0;
		mostExpensiveProduct = Product("", "", 0, 0.0); // Initialize mostExpensiveProduct
		totalItemsInCategory.clear();
		totalValueOfCategory.clear();

		// Initialize highestProductPrice & mostExpensiveProduct with 1st product
		if (!inventoryData.empty()) {
			highestProductPrice = inventoryData[0].getPrice();
			mostExpensiveProduct = inventoryData[0];
		}

		// Perform calculations for each unique product in inventory
		for (const auto& product : inventoryData) {
			double productTotalValue = product.getPrice() * product.getQuantity();	// Get total value for all items of this product type
			totalStockValue += productTotalValue;	// Add product value to total inventory value

			// Update most expensive product if current product price is higher
			if (product.getPrice() > highestProductPrice) {
				highestProductPrice = product.getPrice();
				mostExpensiveProduct = product;
			}

			// Generate category summary
			totalItemsInCategory[product.getCategory()] += product.getQuantity();
			totalValueOfCategory[product.getCategory()] += productTotalValue;
		}
	}

	// Write results to a JSON file.
	void writeToFile(const string& filename) {
		ofstream outputFile{ filename };
		if (!outputFile.is_open()) {
			cerr << "Error while opening output file " << filename << "." << endl;
			throw runtime_error("Could not open output file " + filename + ".");
		}

		// Create a JSON object to store the summary data
		nlohmann::json outputJson;					// Create an empty JSON object
		outputJson["total_stock_value"] = totalStockValue;
		outputJson["most_expensive_product"] = {
			{"name", mostExpensiveProduct.getName() },
			{"price", mostExpensiveProduct.getPrice() }
		};

		// Create a JSON object to store the category summary
		nlohmann::json categorySummary;
		for (const auto& pair : totalItemsInCategory) {
			categorySummary[pair.first] = {
				{"total_items", pair.second },
				{"total_value", totalValueOfCategory[pair.first]}
			};
		}
		outputJson["category_summary"] = categorySummary;

		// Write the JSON object to the output file with indentation and floating-point precision
		stringstream ss;
		ss << fixed << setprecision(2) << outputJson.dump(4) << endl;	// Decimal place formatting does not work
		outputFile << ss.str();
	}
};