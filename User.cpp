#include "User.h"

string User::getUsername() {
	return user;
}

string User::getPassword() {
	return pass;
}

void User::setUsername(string user) {
	this->user = user;
}

void User::setPassword(string pass) {
	this->pass = pass;
}

User* User::findUser(string user) {
	for (int i = 0; i < User::users.size(); i++) {
		if (User::users[i]->getUsername() == user) {
			return users[i];
		}
	}
	return NULL;
}

bool User::usernameTaken(string user) {
	if (findUser(user) != NULL) {
		return true;
	}
	return false;
}

void User::searchBook() {
	cout << "0 -- ISBN" << endl;
	cout << "1 -- Title" << endl;
	cout << "2 -- Author" << endl;
	cout << "3 -- Category" << endl;

	vector<Book*>* books;
	char expression;
	string input;
	cout << "Choose an option to search with: ";
	cin >> expression;

	switch (expression) {
	case '0':
		cout << "Enter the ISBN: " << endl;
		cin >> input;
		books = Book::findBookISBN(input);
		break;
	case '1':
		cout << "Enter the Title: " << endl;
		cin >> input;
		books = Book::findBookTitle(input);
		break;
	case '2':
		cout << "Enter the Author's name: " << endl;
		cin >> input;
		books = Book::findBookAuthor(input);
		break;
	case '3':
		cout << "Enter the Category: " << endl;
		cin >> input;
		books = Book::findBookCategory(input);
		break;
	default:
		cout << "\nError: That is not a valid choice" << endl;
		return;
	}
	cout << endl;

	if (books == NULL) {
		cout << "Error: That book does not exist" << endl;
		return;
	}
	else {
		for (int i = 0; i < books->size(); i++) {
			books->at(i)->print();
		}
	}
}