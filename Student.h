#pragma once
#include "Reader.h"

class Student : public Reader {
public:
	Student();
	Student(string user, string pass);
	Student(string user, string pass, int maxCopies, int maxPeriod, int penalties);
	Student(string user, string pass, int maxCopies, int maxPeriod, int penalties, vector<Copy*>* borrowedCopies, vector<Copy*>* reservedCopies);
	void operator<<(const string fileName);
	void operator>>(const string fileName);
	void print();
};