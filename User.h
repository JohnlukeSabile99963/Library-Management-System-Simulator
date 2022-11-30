#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Book.h"
#include "Copy.h"
using namespace std;

class User {
protected:
	string user, pass;
public:
	static vector<User*> users;
	string getUsername();
	string getPassword();
	void setUsername(string user);
	void setPassword(string pass);

	virtual void operator<<(const string fileName) {};
	virtual void operator>>(const string fileName) {};
	static bool usernameTaken(string user);
	static User* findUser(string user);
	void searchBook();
	virtual void print() {};
};