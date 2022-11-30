#include "Student.h"

Student::Student() {
	user = "";
	pass = "";
	maxCopies = 5;
	maxPeriod = 30;
	penalties = 0;
}

Student::Student(string user, string pass) {
	this->user = user;
	this->pass = pass;
	maxCopies = 5;
	maxPeriod = 30;
	penalties = 0;
}

Student::Student(string user, string pass, int maxCopies, int maxPeriod, int penalties) {
	this->user = user;
	this->pass = pass;
	this->maxCopies = 5;
	this->maxPeriod = 30;
	this->penalties = 0;
}

Student::Student(string user, string pass, int maxCopies, int maxPeriod, int penalties, vector<Copy*>* borrowedCopies, vector<Copy*>* reservedCopies) {
	this->user = user;
	this->pass = pass;
	this->maxCopies = maxCopies;
	this->maxPeriod = maxPeriod;
	this->penalties = penalties;
	this->borrowedCopies = *borrowedCopies;
	this->reservedCopies = *reservedCopies;
}

void Student::operator<<(const string fileName) {
	ifstream in(fileName); //opens student.txt
	if (in.fail()) {
		cout << "Could not open student.txt" << endl; //program terminates if file is unable to be opened
		exit(1);
	}

	string user, pass;
	int maxCopies, maxPeriod, penalties;
	while (in >> user >> pass >> maxCopies >> maxPeriod >> penalties) { //reads in the data for each student from the file
		users.push_back(new Student(user, pass, maxCopies, maxPeriod, penalties)); //adds the student into static vector<User*> users
	}

	in.close(); //close ifstream
}

void Student::operator>>(const string fileName) {
	ofstream out(fileName, ofstream::out | ofstream::trunc); //clears and opens the output file
	if (out.fail()) {
		cout << "Could not open student.txt" << endl; //program terminates if file is unable to be opened
		exit(1);
	}

	for (int i = 0; i < users.size(); i++) { //loop through all users in the system
		if (typeid(*users[i]) == typeid(Student)) { //write data if the user is a student
			Student* student = (Student*)users[i];
			out << student->getUsername() << " " << student->getPassword() << " "
				<< student->getMaxCopies() << " " << student->getMaxPeriod() << " "
				<< student->getPenalties() << endl;
		}
	}

	out.close(); //close ofstream
}

void Student::print() {
	cout << "Username: " << user << endl;
	cout << "Password: " << pass << endl;
	cout << "Type: Student" << endl;
	cout << "Maximum Copies Allowed: " << maxCopies - penalties / 5 << endl;
	cout << "Maximum Borrowing Period: " << maxPeriod << endl;
	cout << "Penalties: " << penalties << endl;
	cout << "------Borrowed Copies------" << endl;
	for (int i = 0; i < borrowedCopies.size(); i++) {
		Book* book = borrowedCopies[i]->getBook();
		cout << "ISBN: " << book->getISBN() << endl;
		cout << "Title: " << book->getTitle() << endl;
		cout << "Author: " << book->getAuthor() << endl;
		cout << "Category: " << book->getCategory() << endl;
		cout << "Index: " << book->getIndex() << endl;
		cout << "Available: " << book->getAvailable() << endl;
		cout << "Favor: " << book->getFavor() << endl;
		borrowedCopies[i]->print();
		cout << endl;
	}
	cout << "------Reserved Copies------" << endl;
	for (int i = 0; i < reservedCopies.size(); i++) {
		Book* book = reservedCopies[i]->getBook();
		cout << "ISBN: " << book->getISBN() << endl;
		cout << "Title: " << book->getTitle() << endl;
		cout << "Author: " << book->getAuthor() << endl;
		cout << "Category: " << book->getCategory() << endl;
		cout << "Index: " << book->getIndex() << endl;
		cout << "Available: " << book->getAvailable() << endl;
		cout << "Favor: " << book->getFavor() << endl;
		reservedCopies[i]->print();
		cout << endl;
	}
}