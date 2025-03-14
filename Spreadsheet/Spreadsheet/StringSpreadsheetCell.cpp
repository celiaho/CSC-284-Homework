import StringSpreadsheetCell;

import SpreadsheetCell;
import <string>;
import <string_view>;
import <optional>;
using namespace std;

void StringSpreadsheetCell::set(string_view value) {
	this->value = string(value);
};

string StringSpreadsheetCell::getString() const {
	return this->value.value_or("");	// If value has a string, return string OR "" (empty string) if empty
}