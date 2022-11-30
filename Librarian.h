#pragma once
#include "User.h"

class Librarian : public User {
public:
	Librarian();
	Librarian(string user, string pass);
	void operator<<(const string fileName);
	void operator>>(const string fileName);
	void addBook(int day);
	void addUser();
	void deleteBook();
	void deleteUser();
	void searchUser();
	void print();
};