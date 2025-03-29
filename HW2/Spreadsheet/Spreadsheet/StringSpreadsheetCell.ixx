export module StringSpreadsheetCell;

import SpreadsheetCell;
import <string>;
import <string_view>;
import <optional>;
using namespace std;

export class StringSpreadsheetCell : public SpreadsheetCell {
private:
	optional<string> value;

public:
	void set(string_view value) override;
	string getString() const override;
};