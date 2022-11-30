#include "Teacher.h"

Teacher::Teacher() {
	user = "";
	pass = "";
	maxCopies = 10;
	maxPeriod = 50;
	penalties = 0;
}

Teacher::Teacher(string user, string pass) {
	this->user = user;
	this->pass = pass;
	maxCopies = 10;
	maxPeriod = 50;
	penalties = 0;
}

Teacher::Teacher(string user, string pass, int maxCopies, int maxPeriod, int penalties) {
	this->user = user;
	this->pass = pass;
	this->maxCopies = 10;
	this->maxPeriod = 50;
	this->penalties = 0;
}

Teacher::Teacher(string user, string pass, int maxCopies, int maxPeriod, int penalties, vector<Copy*>* borrowedCopies, vector<Copy*>* reservedCopies) {
	this->user = user;
	this->pass = pass;
	this->maxCopies = maxCopies;
	this->maxPeriod = maxPeriod;
	this->penalties = penalties;
	this->borrowedCopies = *borrowedCopies;
	this->reservedCopies = *reservedCopies;
}

void Teacher::operator<<(const string fileName) {
	ifstream in(fileName); //opens teacher.txt
	if (in.fail()) {
		cout << "Could not open teacher.txt" << endl; //program terminates if file is unable to be opened
		exit(1);
	}

	string user, pass;
	int maxCopies, maxPeriod, penalties;
	while (in >> user >> pass >> maxCopies >> maxPeriod >> penalties) { //reads in the data for each teacher from the file
		users.push_back(new Teacher(user, pass, maxCopies, maxPeriod, penalties));//adds the teacher into static vector<User*> users
	}

	in.close(); //close ifstream
}

void Teacher::operator>>(const string fileName) {
	ofstream out(fileName, ofstream::out | ofstream::trunc); //clears and opens the output file
	if (out.fail()) {
		cout << "Could not open teacher.txt" << endl; //program terminates if file is unable to be opened
		exit(1);
	}

	for (int i = 0; i < users.size(); i++) { //loop through all users in the system
		if (typeid(*users[i]) == typeid(Teacher)) { //write data if the user is a teacher
			Teacher* teacher = (Teacher*)users[i];
			out << teacher->getUsername() << " " << teacher->getPassword() << " "
				<< teacher->getMaxCopies() << " " << teacher->getMaxPeriod() << " "
				<< teacher->getPenalties() << endl;
		}
	}

	out.close(); //close ofstream
}

void Teacher::print() {
	cout << "Username: " << user << endl;
	cout << "Password: " << pass << endl;
	cout << "Type: Teacher" << endl;
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