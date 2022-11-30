#include "Book.h"
#include "Copy.h"
#include "User.h"
#include "Reader.h"
#include <algorithm>

vector<int> Copy::openIDs;
int Copy::largestID;

Book::Book() {
	ISBN = "";
	title = "";
	author = "";
	category = "";
	index = "";
	available = 0;
	favor = 0;
}

Book::Book(string ISBN, string title, string author, string category, string index) {
	this->ISBN = ISBN;
	this->title = title;
	this->author = author;
	this->category = category;
	this->index = index;
	available = 0;
	favor = 0;
}

Book::Book(string ISBN, string title, string author, string category, string index, int available, int favor) {
	this->ISBN = ISBN;
	this->title = title;
	this->author = author;
	this->category = category;
	this->index = index;
	this->available = available;
	this->favor = favor;
}

Book::Book(string ISBN, string title, string author, string category, string index, int available, int favor, vector<Copy*>* copies) {
	this->ISBN = ISBN;
	this->title = title;
	this->author = author;
	this->category = category;
	this->index = index;
	this->available = available;
	this->favor = favor;
	this->copies = *copies;
}

string Book::getISBN() {
	return ISBN;
}

string Book::getTitle() {
	return title;
}

string Book::getAuthor() {
	return author;
}

string Book::getCategory() {
	return category;
}

string Book::getIndex() {
	return index;
}

vector<Copy*>* Book::getCopies() {
	return &copies;
}

int Book::getAvailable() {
	return available;
}

int Book::getFavor() {
	return favor;
}

void Book::setISBN(string isbn) {
	ISBN = isbn;
}

void Book::setCategory(string category) {
	this->category = category;
}

void Book::setIndex(string index) {
	this->index = index;
}

void Book::setAvailable(int available) {
	this->available = available;
}

void Book::setFavor(int favor) {
	this->favor = favor;
}

void Book::setCopies(vector<Copy*>* copies) {
	this->copies = *copies;
}

void Book::operator<<(const string fileName) {
	ifstream in(fileName); //opens book.txt
	if (in.fail()) { //checks for failure in opening file
		cout << "Could not open book.txt" << endl;
		exit(1); //exit if file could not be opened
	}

	int numberOfOpenIDs, largestID; //read previous ID data
	vector<int>* openIDs = new vector<int>;
	in >> numberOfOpenIDs;
	for (int i = 0; i < numberOfOpenIDs; i++) {
		int id;
		in >> id;
		openIDs->push_back(id);
	}
	in >> largestID;
	Copy::openIDs = *openIDs;
	Copy::largestID = largestID;

	string isbn, title, author, category, index;
	int available, favor, numberOfCopies;
	while (in >> isbn >> title >> author >> category >> index >> available >> favor >> numberOfCopies) {	//loops and reads the data in text file	
		Book* book = new Book(isbn, title, author, category, index, available, favor);

		vector<Copy*> copies;
		for (int i = 0; i < numberOfCopies; i++) { //loops through and reads data for any copies
			int id, dayAvailable, start, exp, numberOfReservations;
			string readerName;
			bool available;
			in >> id >> available >> dayAvailable >> start >> exp >> readerName >> numberOfReservations;
			vector<Reader*>* reservers = new vector<Reader*>;
			vector<int*>* reserveDates = new vector<int*>;

			for (int o = 0; o < numberOfReservations; o++) { //loops through and reads data for any reservations
				string reserverName;
				int* day = new int;
				in >> reserverName >> *day;
				reservers->push_back((Reader*)User::findUser(reserverName));
				reserveDates->push_back(day);
			}

			Reader* reader = (Reader*)User::findUser(readerName);
			Copy* copy = new Copy(id, book, reader, reservers, reserveDates, available, dayAvailable, start, exp); //creates full copy
			copies.push_back(copy);

			if (reader != NULL) {
				reader->getBorrowedCopies()->push_back(copy); //pushes the copy into the appropriate reader
			}
			for (int i = 0; i < copy->getReservers()->size(); i++) {
				copy->getReservers()->at(i)->getReservedCopies()->push_back(copy); //pushes the copy into the appropriate reservers
			}
		}

		book->setCopies(&copies);
		books.push_back(book); //adds a new book to the static vector<Book*> books
	}

	in.close(); //closes text file
}
void Book::operator>>(const string fileName) {
	ofstream out(fileName, ofstream::out | ofstream::trunc); //clears and opens the output file
	if (out.fail()) { //checks for failure in opening file
		cout << "Could not open book.txt" << endl;
		exit(1); //exit if file could not be opened
	}

	out << Copy::openIDs.size() << " "; //write ID data
	for (int i = 0; i < Copy::openIDs.size(); i++) {
		out << Copy::openIDs[i] << " ";
	}
	out << Copy::largestID << endl;

	for (int i = 0; i < books.size(); i++) {
		out << books.at(i)->getISBN() << " " << books.at(i)->getTitle() << " " //writes all the book's information in the output file
			<< books.at(i)->getAuthor() << " " << books.at(i)->getCategory() << " "
			<< books.at(i)->getIndex() << " " << books.at(i)->getAvailable() << " "
			<< books.at(i)->getFavor() << " " << books.at(i)->getCopies()->size() << endl;

		for (int o = 0; o < books.at(i)->getCopies()->size(); o++) { //writes all the copies' information in the output file
			Copy* copy = books.at(i)->getCopies()->at(o);
			string reader = "N/A";
			if (copy->getReader() != NULL) {
				reader = copy->getReader()->getUsername();
			}
			out << copy->getID() << " " << copy->getAvailable() << " " << copy->getDayAvailable() << " "
				<< copy->getStart() << " " << copy->getExp() << " " << reader << " "
				<< copy->getReservers()->size() << endl;

			for (int p = 0; p < copy->getReservers()->size(); p++) { //writes all the reservation information for the copy into the output file
				out << copy->getReservers()->at(p)->getUsername() << " " << *copy->getReserveDates()->at(p) << endl;
			}
		}
	}

	out.close(); //closes text file
}

vector<Book*>* Book::findBookISBN(string ISBN) {
	vector<Book*>* results = new vector<Book*>;

	for (int i = 0; i < books.size(); i++) {
		if (books.at(i)->getISBN() == ISBN) {
			results->push_back(books.at(i));
		}
	}

	if (results->empty()) {
		return NULL;
	}
	else {
		sortPopularity(results);
		return results;
	}
}

vector<Book*>* Book::findBookTitle(string title) {
	vector<Book*>* results = new vector<Book*>;

	for (int i = 0; i < books.size(); i++) {
		if (books.at(i)->getTitle() == title) {
			results->push_back(books.at(i));
		}
	}

	if (results->empty()) {
		return NULL;
	}
	else {
		sortPopularity(results);
		return results;
	}
}

vector<Book*>* Book::findBookAuthor(string author) {
	vector<Book*>* results = new vector<Book*>;

	for (int i = 0; i < books.size(); i++) {
		if (books.at(i)->getAuthor() == author) {
			results->push_back(books.at(i));
		}
	}

	if (results->empty()) {
		return NULL;
	}
	else {
		sortPopularity(results);
		return results;
	}
}

vector<Book*>* Book::findBookCategory(string category) {
	vector<Book*>* results = new vector<Book*>;

	for (int i = 0; i < Book::books.size(); i++) {
		if (books.at(i)->getCategory() == category) {
			results->push_back(books.at(i));
		}
	}

	if (results->empty()) {
		return NULL;
	}
	else {
		sortPopularity(results);
		return results;
	}
}

void Book::sortPopularity(vector<Book*>* books) {
	std::sort(books->begin(), books->end(), popularityOrder);
}

bool Book::popularityOrder(Book* a, Book* b) {
	int popA = 0, popB = 0;

	for (int i = 0; i < a->getCopies()->size(); i++)
		popA += a->getCopies()->at(i)->getReservers()->size();

	for (int i = 0; i < b->getCopies()->size(); i++)
		popB += b->getCopies()->at(i)->getReservers()->size();

	return popA > popB;
}

void Book::addCopy(int day)
{
	Copy* copy = new Copy(this, day);
	if (Copy::openIDs.empty())
	{
		copy->setID(++Copy::largestID);
	}
	else
	{
		copy->setID(Copy::openIDs.back());
		Copy::openIDs.pop_back();
	}
	available++;
	copies.push_back(copy);
	cout << "A new copy of \"" << title << "\" has been created with (ID: " << copy->getID() << ")" << endl;
}

void Book::deleteCopy(int id)
{
	Copy* copy = Copy::findCopy(id);
	while (!copy->getReservers()->empty())
	{
		copy->deleteReserver(copy->getReservers()->front());
	}
	if (copy->getAvailable())
	{
		available--;
	}

	Copy::openIDs.push_back(copy->getID());

	int index;
	for (index = 0; index < copies.size(); index++)
	{
		if (copies.at(index) == copy)
		{
			break;
		}
	}

	copies.erase(copies.begin() + index);
	cout << "Copy of \"" << title << "\" (ID: " << copy->getID() << ") has been deleted" << endl;
	if (copies.empty())
	{
		for (int index = 0; index < books.size(); index++)
		{
			if (books.at(index) == this)
			{
				break;
			}
		}
		books.erase(books.begin() + index);
		cout << "No more copies of \"" << title << "\" exist. The book has been removed from the system." << endl;
	}
}

void Book::print() {
	cout << "------BOOK------" << endl;
	cout << "ISBN: " << ISBN << endl;
	cout << "Title: " << title << endl;
	cout << "Author: " << author << endl;
	cout << "Category: " << category << endl;
	cout << "Index: " << index << endl;
	cout << "Available: " << available << endl;
	cout << "Favor: " << favor << endl;
	cout << "------COPIES------" << endl;
	for (int i = 0; i < copies.size(); i++) {
		copies.at(i)->print();
		cout << endl;
	}
	cout << endl;
}