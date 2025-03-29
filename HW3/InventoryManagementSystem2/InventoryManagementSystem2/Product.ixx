export module Product;

import <string>;
using namespace std;

export class Product {
private:
	string name;
	string category;
	int quantity;
	double price;

public:
	// Cxs
	Product() : name(""), category(""), quantity(0), price(0.0) {}
	/*Product() {		// alt syntax
		name = "";
		category = "";
		quantity = 0;
		price = 0.0;
	}*/
	Product(const string& name, const string& category, int quantity, double price);

	// Getters
	const string getName() const;
	const string getCategory() const;
	int getQuantity() const;
	double getPrice() const;
};