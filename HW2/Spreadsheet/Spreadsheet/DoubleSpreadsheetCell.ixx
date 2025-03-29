export module DoubleSpreadsheetCell;

import SpreadsheetCell;
import <string>;
import <string_view>;
import <optional>;
using namespace std;

export class DoubleSpreadsheetCell : public SpreadsheetCell {
private:
	optional<double> value;

public:
	void set(double value);
	void set(string_view value) override;
	string getString() const override;

	//// static helper fns
	//static doubleToString(double value);
	//staic stringToDouble(string_view value);
};