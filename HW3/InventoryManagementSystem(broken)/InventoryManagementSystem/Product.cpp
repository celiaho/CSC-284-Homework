import Product;

import <string>;
using namespace std;

// Cx
Product::Product(const string& name, const string& category, int quantity, double price) {
	this->name = name;
	this->category = category;
	this->quantity = quantity;
	this->price = price;
}
// Product::Product(const string& name, const string& category, int quantity, double price) : name(name), category(category), quantity(quantity), price(price) {}		// Alternate syntax for cx

// Getters
const string Product::getName() const { return name; }
const string Product::getCategory() const { return category; }
int Product::getQuantity() const { return quantity; }
double Product::getPrice() const { return price; }