#pragma once
#include "Reader.h"

class Teacher : public Reader {
public:
	Teacher();
	Teacher(string user, string pass);
	Teacher(string user, string pass, int maxCopies, int maxPeriod, int penalties);
	Teacher(string user, string pass, int maxCopies, int maxPeriod, int penalties, vector<Copy*>* borrowedCopies, vector<Copy*>* reservedCopies);
	void operator<<(const string fileName);
	void operator>>(const string fileName);
	void print();
};