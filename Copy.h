#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Book;
class Reader;
class Copy {
private:
	int id, dayAvailable, start, exp;
	Book* book;
	Reader* reader;
	vector<Reader*> reservers;
	vector<int*> reserveDates;
	bool available;
public:
	static vector<int> openIDs;
	static int largestID;
	Copy();
	Copy(Book* book, int dayAvailable);
	Copy(int id, Book* book, Reader* reader, bool available, int dayAvailable, int start, int exp);
	Copy(int id, Book* book, Reader* reader, vector<Reader*>* reservers, vector<int*>* reserveDates, bool available, int dayAvailable, int start, int exp);
	int getID();
	Book* getBook();
	Reader* getReader();
	vector<Reader*>* getReservers();
	bool getAvailable();
	int getDayAvailable();
	int getStart();
	vector<int*>* getReserveDates();
	int getExp();
	void setID(int id);
	void setBook(Book* book);
	void setReader(Reader* reader);
	void setReservers(vector<Reader*>* reservers);
	void setAvailable(bool available);
	void setDayAvailable(int dayAvailable);
	void setStart(int start);
	void setReserveDates(vector<int*>* reserveDates);
	void setExp(int exp);

	static void updateReservers(int day);
	static Copy* findCopy(int id);
	void addReserver(Reader* reserver, int day);
	void deleteReserver(Reader* reserver);
	void print();
};