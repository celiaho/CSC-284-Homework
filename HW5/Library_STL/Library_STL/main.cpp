/*
Description:This program manages a collection of books in a library. The library uses containers from the Standard Library, iterators, and ranges for efficient data manipulation and retrieval.
Requirements: https://github.com/pradeepselvakumar/CSC-284/blob/main/SessionNine/LibraryRequirements.md
*/
import Library;
import Book;
import <iostream>;
import <vector>;
using namespace std;

int main() {
    Library myLibrary;

    // Create Book objects
    Book book1("Starting Out with C++ from Control Structures Through Objects", "Tony Gaddis", 2018, "Textbook");
    Book book2("The 4-Hour Work Week: Escape the 9 - 5, Live Anywhere and Join the New Rich", "Timothy Ferriss", 2007, "Self-Help");
    Book book3("Siddhartha", "Hermann Hesse", 1922, "Historical Fiction");
    Book book4("In Cold Blood", "Truman Capote", 1965, "Nonfiction");
    Book book5("The Omnivore's Dilemma: A Natural History of Four Meals", "Michael Pollan", 2006, "Nonfiction");
    Book book6("Fast Food Nation: The Dark Side of the All-American Meal", "Eric Schlosser", 2001, "Nonfiction");
    Book book7("Vintage Murakami", "Haruki Murakami", 2004, "Short Stories");
    Book book8("Eat, Pray, Love", "Elizabeth Gilbert", 2006, "Autobiographical Fiction");
    Book book9("Snow Falling on Cedars", "David Guterson", 1994, "Historical Fiction");
    Book book10("Kitchen", "Banana Yoshimoto", 1988, "Fiction");

    
    /********** Demonstrate Container Operations **********/
    
    // Add books to the library
    myLibrary.add_book(book1);
    myLibrary.add_book(book2);
    myLibrary.add_book(book3);
    myLibrary.add_book(book4);
    myLibrary.add_book(book5);
    myLibrary.add_book(book6);
    myLibrary.add_book(book7);
    myLibrary.add_book(book8);
    myLibrary.add_book(book9);
    myLibrary.add_book(book10);

    cout << "Books have been added to Celia's library:\n";
    myLibrary.display_all_books_with_general_iterator(); 

    // Remove books using erase() and remove_if()
    myLibrary.remove_book("Kitchen");
    myLibrary.remove_book("Snow Falling on Cedars");
    
    myLibrary.display_last_removed_books();
    
    cout << "Here is the current library:\n";
    myLibrary.display_all_books_with_general_iterator();

    // Sort books by year in ascending order
    cout << "Sorting books by year in ascending order:\n";
    myLibrary.sort_by_year(true);
    myLibrary.display_all_books_with_general_iterator();

    // Sort books by year in ascending order
    cout << "Sorting books by year in descending order:\n";
    myLibrary.sort_by_year(false);
    myLibrary.display_all_books_with_general_iterator();

    // Demonstrate the use of std::find() to locate books by title
    string title_to_find = "Siddhartha";
    optional<Book> book_search_result = myLibrary.find_book(title_to_find);

    if (book_search_result.has_value()) {
        cout << "Found book with title '" << title_to_find << "' using std::find:\n";
        cout << book_search_result.value().display_book() << "\n\n";
    }
    else {
        cout << "Book with title '" << title_to_find << "' not found using std::find.\n";
    }

    // Demonstrate const_iterator traversing books without modifying them
    cout << "Displaying all books without modifying them using a const_iterator:\n";
    for (Library::const_iterator cit = myLibrary.cbegin(); cit != myLibrary.cend(); ++cit) {
        cout << cit->display_book() << "\n";
    }
    cout << "\n";

    // Demonstrate using reverse iterators to display books in reverse order
    cout << "Displaying books in reverse order using reverse iterators:\n";
    // Use the display_all_books_reverse() function we added
    myLibrary.display_all_books_reverse();
};