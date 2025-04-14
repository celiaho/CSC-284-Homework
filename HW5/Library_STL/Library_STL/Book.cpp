/*
    Create a class Book with the following attributes:
        title (string)
        author (string)
        year (integer)
        genre (string)
    Add a constructor to initialize these values.
    Implement a method to display the book details in the format: "Title by Author (Year) [Genre]".
*/
import Book;

import <string>;
import <sstream>; // To create strings from different data types
using namespace std;

Book::Book(string title, string author, int year,  string genre) : title(title), author(author), year(year), genre(genre) {}

string Book::get_title() const { return title; }
string Book::get_author() const { return author; }
int Book::get_year() const { return year; }
string Book::get_genre() const { return genre; }

string Book::display_book() const {
    stringstream ss;
    ss << "  - " << title << " by " << author << " (" << year << ") [" << genre << "]";
    return ss.str();
}

// Implement overloaded equality operator to compare Book objects (for using std::find() to locate books by title)
bool Book::operator==(const Book& other) const {
    return title == other.title;
}