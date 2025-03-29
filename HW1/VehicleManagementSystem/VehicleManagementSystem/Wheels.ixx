export module Wheels;

import <string>;
using namespace std;

export class Wheels {
private:
	string seasonType;

public:
	Wheels() : Wheels("All-Season") {};

	Wheels(string seasonType) {
		this->seasonType = seasonType;
	};

	string getSeasonType() const {
		return seasonType;
	}
};