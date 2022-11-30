#include "Copy.h"
#include "Book.h"
#include "Reader.h"

Copy::Copy() {
	id = -1;
	book = NULL;
	reader = NULL;
	available = true;
	dayAvailable = -1;
	start = -1;
	exp = -1;
}

Copy::Copy(Book* book, int dayAvailable) {
	id = -1;
	this->book = book;
	reader = NULL;
	available = true;
	this->dayAvailable = dayAvailable;
	start = -1;
	exp = -1;
}

Copy::Copy(int id, Book* book, Reader* reader, bool available, int dayAvailable, int start, int exp) {
	this->id = id;
	this->book = book;
	this->reader = reader;
	this->available = available;
	this->dayAvailable = dayAvailable;
	this->start = start;
	this->exp = exp;
}

Copy::Copy(int id, Book* book, Reader* reader, vector<Reader*>* reservers, vector<int*>* reserveDates, bool available, int dayAvailable, int start, int exp) {
	this->id = id;
	this->book = book;
	this->reader = reader;
	this->reservers = *reservers;
	this->reserveDates = *reserveDates;
	this->available = available;
	this->dayAvailable = dayAvailable;
	this->start = start;
	this->exp = exp;
}

int Copy::getID() {
	return id;
}

Book* Copy::getBook() {
	return book;
}

Reader* Copy::getReader() {
	return reader;
}

vector<Reader*>* Copy::getReservers() {
	return &reservers;
}

bool Copy::getAvailable() {
	return available;
}

int Copy::getDayAvailable() {
	return dayAvailable;
}

int Copy::getStart() {
	return start;
}

vector<int*>* Copy::getReserveDates() {
	return &reserveDates;
}

int Copy::getExp() {
	return exp;
}

void Copy::setID(int id) {
	this->id = id;
}

void Copy::setBook(Book* book) {
	this->book = book;
}

void Copy::setReader(Reader* reader) {
	this->reader = reader;
}

void Copy::setReservers(vector<Reader*>* reservers) {
	this->reservers = *reservers;
}

void Copy::setAvailable(bool available) {
	this->available = available;
	if (available) {
		book->setAvailable(book->getAvailable() + 1);
	}
	else {
		book->setAvailable(book->getAvailable() - 1);
	}
}

void Copy::setDayAvailable(int dayAvailable) {
	this->dayAvailable = dayAvailable;
}

void Copy::setStart(int start) {
	this->start = start;
}

void Copy::setReserveDates(vector<int*>* reserveDates) {
	this->reserveDates = *reserveDates;
}

void Copy::setExp(int exp) {
	this->exp = exp;
}

void Copy::updateReservers(int day) {
	for (int i = 0; i < Book::books.size(); i++) {
		Book* book = Book::books[i];
		for (int o = 0; o < book->getCopies()->size(); o++) {
			Copy* copy = book->getCopies()->at(o);
			if (copy->available && day > copy->dayAvailable + 5 && !copy->reservers.empty()) {
				cout << copy->reservers.front()->getUsername() << " has been removed from the reservation list for \"" << book->getTitle() << "\" (ID: " << copy->id << ")" << endl;
				copy->deleteReserver(copy->reservers.front());
			}
		}
	}
}

Copy* Copy::findCopy(int id) {
	for (int i = 0; i < Book::books.size(); i++) {
		for (int j = 0; j < Book::books.at(i)->getCopies()->size(); j++) {
			if (id == Book::books.at(i)->getCopies()->at(j)->getID())
				return Book::books.at(i)->getCopies()->at(j);
		}
	}
	return NULL;
}

void Copy::addReserver(Reader* reader, int day) {
	reservers.push_back(reader);
	reserveDates.push_back(new int(day));
}

void Copy::deleteReserver(Reader* reader)
{
	int index;
	for (index = 0; index < reservers.size(); index++)
	{
		if (reservers.at(index) == reader)
		{
			break;
		}
	}

	reservers.erase(reservers.begin() + index);
	reserveDates.erase(reserveDates.begin() + index);

	if (reservers.empty()) {
		cout << "Reservation list for \"" << book->getTitle() << "\" (ID: " << id << ") is now empty" << endl;
	}
}

void Copy::print()
{
	cout << "ID: " << id << endl;
	cout << "Reader: ";
	if (reader != NULL) {
		cout << reader->getUsername() << endl;
	}
	else {
		cout << "N/A" << endl;
	}
	cout << "Reservers: " << endl;
	for (int i = 0; i < reservers.size(); i++)
		cout << reservers.at(i)->getUsername() << "\t\t|\t\tReserved on Day: " << *reserveDates.at(i) << endl;
	cout << "Available: " << boolalpha << available << endl;
	cout << "Last Available on Day: " << dayAvailable << endl;
	cout << "Borrowed on Day: " << getStart() << endl;
	cout << "Expires on Day: " << getExp() << endl;
}