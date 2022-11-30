#pragma once
#include "User.h"

class Reader : public User {
protected:
	int maxCopies, maxPeriod, penalties;
	vector<Copy*> borrowedCopies, reservedCopies;
public:
	int getMaxCopies();
	int getMaxPeriod();
	vector<Copy*>* getBorrowedCopies();
	vector<Copy*>* getReservedCopies();
	int getPenalties();
	void setMaxCopies(int maxCopies);
	void setMaxPeriods(int maxPeriod);
	void setBorrowedCopies(vector<Copy*>* borrowedCopies);
	void setReservedCopies(vector<Copy*>* reservedCopies);
	void setPenalties(int penalties);

	void borrowCopy(int day);
	void returnCopy(int day);
	void reserveCopy(int day);
	void cancelReserveCopy();
	void renewCopy(int day);
	bool overdue(int day);
	static void updatePenalty(int day);
};