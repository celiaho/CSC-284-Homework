import Library;

import Book;
import <string>;
import <vector>;
import <optional>;
import <iostream>;
import <algorithm>;	// for copy_if, ranges::copy, ranges::sort
import <ranges>;	// for views namespace incl. views::filter, views::transform
import <deque>;
using namespace std;

/******************** Library class member functions ********************/
// Adds a new book to the library
void Library::add_book(const Book& book) { 
	books.push_back(book); 
}

// Removes a book from the library by title
void Library::remove_book(const std::string& title) { 
	// Find all books with the matching title
	auto it_begin = remove_if(books.begin(), books.end(),
		[&](const Book& b) { return b.get_title() == title; });

	// Iterate through the books to be removed (from it_begin to books.end())
	for (auto it = it_begin; it != books.end(); ++it) {
		last_removed_books.push_back(*it); // Add the book to last_removed_books deque
	}

	books.erase(it_begin, books.end());	// Erase removed elements from the vector
}

// Searches for a book by title and returns it (if found) or prints an error message
// std::find works via direct value matching for known values
optional<Book> Library::find_book(const string& title) const {
	// Create a temporary Book object with the title we are searching for.
	Book book_to_find(title, "", 0, "");

	// Use std::find to search for a Book with the matching title (using overloaded operator== in Book class)
	auto it = find(books.cbegin(), books.cend(), book_to_find);

	if (it != books.cend()) {	// If iterator isn't at end of books vector, a book with the matching title was found
		return *it;	// Return the found Book object wrapped in an optional
	}
	else {
		cout << "Book with title '" << title << "' not found using std::find.\n";
		return nullopt;	// Return an empty optional to indicate book was not found
	}
}

// std::find_if is more flexible--can search based on any condition
optional<Book> Library::find_book_with_std_find_if(const std::string& title) const {
	auto it = find_if(books.cbegin(), books.cend(),
		[&](const Book& b) { return b.get_title() == title; });
	if (it != books.cend()) {
		return *it;
	} else {
		cout << "ERROR: Book with title '" << title << "' not found using std::find_if.\n";
		return nullopt;
	}
}

// Returns a list of books by a specific author
vector<Book> Library::get_books_by_author(const std::string& author) const {
	vector<Book> result; // Vector to store books by the given author
	// Copy books by the given author to the back of the result vector
	copy_if(books.cbegin(), books.cend(), back_inserter(result),
		[&](const Book& b) { return b.get_author() == author; });
	return result; // Return the vector with books by the given author
}

// Returns a list of books that belong to a specific genre
vector<Book> Library::get_books_by_genre(const std::string& genre) const {
	vector<Book> result;	// Vector to store books of the given genre
	copy_if(books.cbegin(), books.cend(), back_inserter(result),
		[&](const Book& b) { return b.get_genre() == genre; });
	return result;	// Return the vector with books of the given genre
}

/******************** Demonstrate iterators ********************/
// Demonstrate the use of iterators to traverse and display all books in the library
void Library::display_all_books_with_general_iterator() const {
	cout << "All Books (General Iterator):\n";
	for (auto cit = books.cbegin(); cit != books.cend(); ++cit) {
		cout << cit->display_book() << "\n";	// -> = Deref *cit and access mbr fn
	}
	cout << endl;
}

// Use a std::vector<Book>::iterator to iterate over the books in the container and print the book details
void Library::display_all_books_with_iterator() {
	cout << "All Books (std::iterator):\n";
	for (vector<Book>::iterator it = books.begin(); it != books.end(); ++it) {
		cout << it->display_book() << endl;	// -> = Deref *it and access mbr fn
	}
	cout << endl;
}

// Display the books that were last removed
void Library::display_last_removed_books() const {
	if (!last_removed_books.empty()) {
		cout << "These books have been removed from the library:\n";
		for (const auto& book : last_removed_books) {
			cout << book.display_book() << "\n";
		}
	} else {
		cout << "No books were removed in the last operation.\n";
	}
	cout << endl;
}


// Implement an iterator-based search function to find books by author
vector<vector<Book>::iterator> Library::find_books_by_author_iterator(const string& author) {
	vector<vector<Book>::iterator> found_iterators;	// List of iterators that each point to a Book object in books vector with the given author
	for (vector<Book>::iterator it = books.begin(); it != books.end(); ++it) {
		if (it->get_author() == author) {
			found_iterators.push_back(it);	// Add current it iterator to found_iterators vector
		}
	}
	return found_iterators;
}

/******************** Demonstrate Ranges *******************/

// Implement a function that filters books by year (e.g., books published after a certain year) using the ranges library
vector<Book> Library::filter_books_by_year_range(int year) const {
	vector<Book> filtered_books;	// Vector to store filtered books

	// Create a view of the books vector for book with years > the given year
	auto year_view = books | views::filter([year](const Book& book) {	// ????? WHY DOES INTELLISENSE MISTAKENLY SHOW ERRORS FOR STD::RANGES? PROGRAM RUNS FINE.
		return book.get_year() > year;    // Lambda predicate: returns true if the book's year is after the specified year
		});
		
	// Copy year_view elements into the filtered_books vector
	ranges::copy(year_view, back_inserter(filtered_books));	// ????? WHY DOES INTELLISENSE MISTAKENLY SHOW ERRORS FOR STD::RANGES? PROGRAM RUNS FINE.

	return filtered_books; // Return the vector of filtered books
}

// Use std::ranges::filter to filter books based on genre
vector<Book> Library::filter_books_by_genre_range(string genre) const {
	vector<Book> filtered_books;	// Vector to store filtered books

	// Create a view of the books vector with books of the given genre
	auto genre_view = books | views::filter([genre](const Book& book) {
		return book.get_genre() == genre;
		}); 

	// Copy year_view elements into the filtered_books vector
	ranges::copy(genre_view, back_inserter(filtered_books));

	return filtered_books; // Return the vector of filtered books
}

// Create a pipeline of operations to display books from a specific genre and sort them by year using std::ranges::sort and std::ranges::transform
void Library::display_genre_sorted_by_year_range(const string& genre) const {
	// Create a view of the books vector with books of the given genre
	auto genre_view = books | views::filter([genre](const Book& book) {
		return book.get_genre() == genre;
	});

	// Turn genre_view into a vector for ranges::sort sorting
	vector<Book> genre_books(genre_view.begin(), genre_view.end());

	// Sort genre_books vector by year in ascending order using ranges::sort and lambda comparator
	ranges::sort(genre_books, [](const Book& b1, const Book& b2) {
		// If b1 year < b2 year, return true and place b1 before b2, otherwise place b1 after/at same position as b2
		return b1.get_year() < b2.get_year(); 
	});

	// Transform the sorted books into strings using display_book()
	auto display_view = genre_books	// Input the genre_books vector (now sorted by year)
		| views::transform([](const Book& book) {	// Apply "transform" view
			return book.display_book();	// Transformation = Call display_book() on each Book, returning a string
		});	// Output a view of strings

	// Iterate through the transformed view and display the strings
	cout << "Books in genre '" << genre << "' sorted by year (using transform for display):\n";
	for (const auto& book_string : display_view) {
		cout << book_string << endl;
	}
	cout << endl;
}

/******************** Use Container Operations *******************/
void Library::sort_by_year(bool ascending) {
	if (ascending) {
		std::sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
			return a.get_year() < b.get_year();
			});
	} else {
		std::sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
			return a.get_year() > b.get_year();
			});
	}
}

/******************** Advanced Task *******************/
void Library::display_all_books_reverse() const {
	cout << "All Books in reverse order:\n";
	// Iterate through books vector in reverse using constant reverse iterators
	for (auto rit = books.crbegin(); rit != books.crend(); ++rit) {
		cout << rit->display_book() << "\n";
	}
	cout << "\n";
}