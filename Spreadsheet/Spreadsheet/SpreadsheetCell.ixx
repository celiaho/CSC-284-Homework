export module SpreadsheetCell;

import <string>;
import <string_view>;
using namespace std;

export class SpreadsheetCell {
	public:
		virtual ~SpreadsheetCell() = default;   // virtual destructor
		virtual void set(std::string_view value) = 0;
		virtual string getString() const = 0;
	};
