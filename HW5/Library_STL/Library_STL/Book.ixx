export module Book;

import <string>;
using namespace std;

export class Book {
private:
	string title;
	string author;
	int year;
	string genre;

public:
	Book(string title, string author, int year, string genre);

	string get_title() const;
	string get_author() const;
	int get_year() const;
	string get_genre() const;

	string display_book() const;

	// Overload equality operator to compare Book objects (for using std::find() to locate books by title)
	bool operator==(const Book& other) const {
		return title == other.title;
	}
};