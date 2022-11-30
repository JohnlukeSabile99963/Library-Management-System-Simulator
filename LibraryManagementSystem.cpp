#include "Teacher.h"
#include "Student.h"
#include "Librarian.h"
#include "Book.h"
#include "Copy.h"
#include <ctime>
#include <algorithm>
#include <conio.h>

const int lengthOfDay = 5; //length in seconds of 1 day
vector<Book*> Book::books;
vector<User*> User::users;
void loadData();
void saveData();
void countDays(int* totalDays, clock_t startTime);
bool login(User*& user);
void changePassword(User* user);
void printMenu(Librarian* librarian, int day);
void printMenu(Reader* reader, int day);
void choice(char in, Librarian* librarian, int day);
void choice(char in, Reader* reader, int day);
void printAllBooks();
void recommendBooks(Reader* reader);
bool alphabeticalOrder(string a, string b);

int main() {
	clock_t startTime = clock(); //start time of the system
	int day = 0; //total day counter
	char input = -1;
	User* user;
	loadData();

	while (!login(user)); //loop until a valid User has been logged in

	cout << "Welcome to The Library!" << endl;
	string userType = typeid(*user).name();
	cout << "Welcome back, " << userType.substr(6, userType.size()) << endl << endl;
	if (typeid(*user) == typeid(Librarian)) {
		while (input != '0') { //primary program loop
			printMenu((Librarian*)user, day);
			cin >> input;
			cout << endl;

			choice(input, (Librarian*)user, day);
			countDays(&day, startTime);
			Reader::updatePenalty(day);
			Copy::updateReservers(day);
		}
	}
	else {
		recommendBooks((Reader*)user);
		while (input != '0') { //primary program loop
			printMenu((Reader*)user, day);
			cin >> input;
			cout << endl;

			choice(input, (Reader*)user, day);
			countDays(&day, startTime);
			Reader::updatePenalty(day);
			Copy::updateReservers(day);
		}
	}

	saveData();
	return 0;
}

void loadData() {
	*(new Librarian()) << "librarian.txt"; //load librarians
	*(new Teacher()) << "teacher.txt"; //load teachers
	*(new Student()) << "student.txt"; //load students
	*(new Book()) << "book.txt"; //load books
}

void saveData() {
	*(new Librarian()) >> "librarian.txt"; //save librarians
	*(new Teacher()) >> "teacher.txt"; //save teachers
	*(new Student()) >> "student.txt"; //save students
	*(new Book()) >> "book.txt"; //save books
}

void countDays(int* totalDays, clock_t startTime) {
	int daysPassed = *totalDays;
	int totalSeconds = ((float)(clock() - startTime)) / CLOCKS_PER_SEC;
	*totalDays = totalSeconds / lengthOfDay;

	daysPassed = *totalDays - daysPassed; //subtracts new value for totalDays with the old value for totalDays

	if (daysPassed != 0) { //if any days have passed
		cout << daysPassed << " Day(s) have passed..." << endl;
	}
}

bool login(User*& user) {
	string username, pass = "";
	char c;
	cout << "Username: ";
	cin >> username;
	cout << "Password: ";
	while ((c = _getch()) != '\r') {
		pass += c;
		cout << "*";
	}
	cout << endl;

	user = User::findUser(username); //points the user to a User with a matching username
	if (user != NULL && user->getPassword() == pass) { //if a valid user was found and the password is correct
		cout << "Login successful!\n" << endl;
		return true;
	}
	cout << "Username or Password incorrect. Please login again.\n" << endl;
	return false;
}

void changePassword(User* user) {
	string pass;
	cout << "Enter new password: ";
	cin >> pass;
	user->setPassword(pass);
}

void printMenu(Librarian* librarian, int day) {
	cout << "Please choose (Day " << day << "): " << endl;
	cout << "\t\t1 -- Search Books" << endl;
	cout << "\t\t2 -- Add Books" << endl;
	cout << "\t\t3 -- Delete Books" << endl;
	cout << "\t\t4 -- Search Users" << endl;
	cout << "\t\t5 -- Add Users" << endl;
	cout << "\t\t6 -- Delete Users" << endl;
	cout << "\t\t7 -- My Information" << endl;
	cout << "\t\t8 -- Display All Books" << endl;
	cout << "\t\t9 -- Change Password" << endl;
	cout << "\t\t0 -- Log Out" << endl;
}

void printMenu(Reader* reader, int day) {
	cout << "You have " << reader->getPenalties() << " penalties." << endl;
	cout << "Please choose (Day " << day << "): " << endl;
	cout << "\t\t1 -- Search Books" << endl;
	cout << "\t\t2 -- Borrow Books" << endl;
	cout << "\t\t3 -- Return Books" << endl;
	cout << "\t\t4 -- Reserve Books" << endl;
	cout << "\t\t5 -- Cancel Reservations" << endl;
	cout << "\t\t6 -- Renew Books" << endl;
	cout << "\t\t7 -- My Information" << endl;
	cout << "\t\t8 -- Display All Books" << endl;
	cout << "\t\t9 -- Change Password" << endl;
	cout << "\t\t0 -- Log Out" << endl;
}

void choice(char in, Librarian* librarian, int day) {
	switch (in) { //looks for the corresponding choice and executes their function
	case '1':
		librarian->searchBook();
		break;
	case '2':
		librarian->addBook(day);
		break;
	case '3':
		librarian->deleteBook();
		break;
	case '4':
		librarian->searchUser();
		break;
	case '5':
		librarian->addUser();
		break;
	case '6':
		librarian->deleteUser();
		break;
	case '7':
		librarian->print();
		break;
	case '8':
		printAllBooks();
		break;
	case '9':
		changePassword(librarian);
		break;
	case '0':
		cout << "Logging out..." << endl;
		break;
	default: //if the user enters a choice that is not between 0-9
		cout << "Error: Invalid choice" << endl;
	}
	cout << endl;
}

void choice(char in, Reader* reader, int day) {
	switch (in) { //looks for the corresponding choice and executes their function
	case '1':
		reader->searchBook();
		break;
	case '2':
		reader->borrowCopy(day);
		break;
	case '3':
		reader->returnCopy(day);
		break;
	case '4':
		reader->reserveCopy(day);
		break;
	case '5':
		reader->cancelReserveCopy();
		break;
	case '6':
		reader->renewCopy(day);
		break;
	case '7':
		reader->print();
		break;
	case '8':
		printAllBooks();
		break;
	case '9':
		changePassword(reader);
		break;
	case '0':
		cout << "Logging out..." << endl;
		break;
	default: //if the user enters a choice that is not between 0-9
		cout << "Error: Invalid choice" << endl;
	}
	cout << endl;
}

void printAllBooks() {
	for (int i = 0; i < Book::books.size(); i++) { //loops through all Book objects in static vector<Book*> books
		Book::books.at(i)->print(); //prints the Book information
		cout << endl;
	}
}

void recommendBooks(Reader* reader) {
	vector<string> categories; //vector used to store all categories that the Student is currently borrowing
	for (int i = 0; i < reader->getBorrowedCopies()->size(); i++) { //loops through the borrowedCopies vector in the provided Student
		categories.push_back(reader->getBorrowedCopies()->at(i)->getBook()->getCategory()); //pushes the category of the Book from borrowedCopies into categories
	}

	string current = ""; //variables to keep track of occurences and the category
	string mostRead = "";
	int count = 0;
	int maxCount = 0;
	std::sort(categories.begin(), categories.end(), alphabeticalOrder); //sorts the categories vector
	for (int i = 0; i < categories.size(); i++) { //loops through the categories vector
		if (i == 0) { //if this is the first iteration of the loop
			current = categories[i]; //sets counter and category variables
			mostRead = categories[i];
			count++;
			maxCount++;
		}
		else if (categories[i] != current) { //if the current category is different than the previous category read
			current = categories[i]; //reset the current category to the one being read
			count = 1; //reset the counter
		}
		else if (categories[i] == current) { //if the current category is the same as the previous category read
			count++; //increment counter
			if (count > maxCount) { //if counter is greater than the all-time max
				mostRead = current; //set the most read category to the current category
				maxCount = count; //set the all-time max to the current counter
			}
		}
	}

	string recommendations = ""; //string storing all recommended books
	for (int i = 0; i < Book::books.size(); i++) { //loops through all Book objects in static vector<Book*> books
		if (Book::books.at(i)->getCategory() == mostRead) { //if the Book has the same category as the Student's most read category
			bool alreadyRead = false; //boolean representing if the student is currently borrwing the Book
			for (int o = 0; o < reader->getBorrowedCopies()->size(); o++) { //loops through the borrowedCopies vector in the Student
				if (reader->getBorrowedCopies()->at(o)->getBook()->getTitle() == Book::books.at(i)->getTitle()) { //if the Student is currently borrowing the Book
					alreadyRead = true; //flag the book
					break; //exit the loop
				}
			}
			if (!alreadyRead && recommendations.find(Book::books.at(i)->getTitle()) == std::string::npos) { //if the Book is not flagged and the Book is not already in the recommendations
				recommendations += Book::books.at(i)->getTitle() + ", "; //add the Book to the recommendations
			}
		}
	}
	if (recommendations != "") { //if recommendations is not empty (There are books to recommend)
		cout << "We recommend you read: " << recommendations.substr(0, recommendations.size() - 2) << endl; //display the recommended books
	}
}

bool alphabeticalOrder(string a, string b) { //returns true if a is alphabetically less than b
	return a < b;
}