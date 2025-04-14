export module Library;

import Book;
import <string>;
import <vector>;
import <optional>;
import <deque>;
using namespace std;

export class Library {
private:
	vector<Book> books;
	deque<Book> last_removed_books;

public:
	void add_book(const Book& book);
	void remove_book(const std::string& title);
	optional<Book> find_book(const string& title) const;	// Using std::find per project instructions; works via direct value matching for known values
	optional<Book> find_book_with_std_find_if(const std::string& title) const;	// Using std::find_if is more flexible
	vector<Book> get_books_by_author(const std::string& author) const;
	vector<Book> get_books_by_genre(const std::string& genre) const;

	void display_all_books_with_general_iterator() const;
	void display_all_books_with_iterator();
	vector<vector<Book>::iterator> find_books_by_author_iterator(const string& author);
	void display_last_removed_books() const;

	vector<Book> filter_books_by_year_range(int year) const;
	vector<Book> filter_books_by_genre_range(string genre) const;
	void display_genre_sorted_by_year_range(const string& genre) const;

	void sort_by_year(bool ascending = true);

	void display_all_books_reverse() const;


    // const_iterator implementation for read-only book traversal
    using const_iterator = vector<Book>::const_iterator;	// Create const_iterator alias
    const_iterator cbegin() const { return books.cbegin(); } // Returns const_iterator pointing to beginning of vector
    const_iterator cend() const { return books.cend(); } // Returns const_iterator pointing to one position past the end of the vector

    // Reverse iterator to display books in reverse order
    using reverse_iterator = vector<Book>::reverse_iterator; // Create reverse_iterator alias
    reverse_iterator rbegin() { return books.rbegin(); } // Returns a reverse iterator pointing to the last element of the vector (the reverse beginning)
    reverse_iterator rend() { return books.rend(); } // Returns a reverse iterator pointing to one position before the beg of the vector (the reverse end)

	using const_reverse_iterator = std::vector<Book>::const_reverse_iterator;
	const_reverse_iterator crbegin() const { return books.crbegin(); } // Returns a constant reverse iterator (read-only) pointing to the last element
    const_reverse_iterator crend() const { return books.crend(); } // Returns a constant reverse iterator (read-only) pointing to one position before the beginning
};