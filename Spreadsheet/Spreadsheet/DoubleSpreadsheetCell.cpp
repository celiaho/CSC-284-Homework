import DoubleSpreadsheetCell;

import SpreadsheetCell;
import <string>;
import <string_view>;
import <optional>;
using namespace std;

// Static helper functions (outside the class)
static string doubleToString(double value) {
	// Check if the double is an integer
	if (floor(value) == value) {	// floor(value) returns the largest integer so if it == value, there are no fractional parts
		return to_string(static_cast<int>(value));	// truncate decimal part & convert to string
	}
	else {	// if double isn't an integer
		return to_string(value);	// convert to string and keep decimal part
	}
}

static double stringToDouble(string_view value) {
	// Convert string_view to string then double
	return stod(string(value));		// stod() = string to double, takes a string, tries to parse into a floating point # (must be w/in double range), converts to double or throws e otherwise
}

// Functions
void DoubleSpreadsheetCell::set(double value) {
	this->value = double(value);
}

void DoubleSpreadsheetCell::set(string_view value) {
	this->value = stringToDouble(value);
}

string DoubleSpreadsheetCell::getString() const {
	if (this->value.has_value()) {
		return doubleToString(this->value.value());
	}
	else {
		return "";
	}
}
