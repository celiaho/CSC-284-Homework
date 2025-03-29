/* This file demonstrates polymorphism in a test program */
import SpreadsheetCell;
import StringSpreadsheetCell;
import DoubleSpreadsheetCell;
import <vector>;
import <memory>;	// for unique_ptr
import <iostream>;
using namespace std;

int main() {

	// Create a vector<unique_ptr<SpreadsheetCell>> to store different cell types
	// This creates a dynamic collection of spreadsheet cells (of various types)
	vector<unique_ptr<SpreadsheetCell>> cellVector;

	// Add 2 StringSpreadsheetCell objects and 1 DoubleSpreadsheetCell object to the vector.
	cellVector.push_back(make_unique<StringSpreadsheetCell>());
	cellVector.push_back(make_unique<StringSpreadsheetCell>());
	cellVector.push_back(make_unique<DoubleSpreadsheetCell>());
	// Other methods emplace_back(index, value) - directly places value at specified index, insert(index, value) - inserts value before value at specified index

// Set cell values with set()
	cellVector[0]->set("hello");
	cellVector[1]->set("10");
	// Explicitly call set(double) bc compiler tries to use overridden set(string_view) method by default
	dynamic_cast<DoubleSpreadsheetCell*>(cellVector[2].get())->set(18.0);

	// Print cell values with getString() to demo dynamic dispatch
	cout << "Demonstrated output:\tVector : [";
	for (size_t i = 0; i < cellVector.size(); ++i) {
		cout << cellVector[i]->getString();
		if (i < cellVector.size() - 1) {
			cout << ", ";
		}
	}
	cout << "]" << endl;
	cout << "Expected output:\tVector : [hello, 10, 18]" << endl;

	return 0;
}