#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Copy;
class Book {
private:
	string ISBN, title, author, category, index;
	int available, favor;
	vector<Copy*> copies;
public:
	static vector<Book*> books;
	Book();
	Book(string ISBN, string title, string author, string category, string index);
	Book(string ISBN, string title, string author, string category, string index, int available, int favor);
	Book(string ISBN, string title, string author, string category, string index, int available, int favor, vector<Copy*>* copies);
	string getISBN();
	string getTitle();
	string getAuthor();
	string getCategory();
	string getIndex();
	vector<Copy*>* getCopies();
	int getAvailable();
	int getFavor();
	void setISBN(string isbn);
	void setCategory(string category);
	void setIndex(string index);
	void setAvailable(int available);
	void setFavor(int favor);
	void setCopies(vector<Copy*>* copies);

	void operator<<(const string fileName);
	void operator>>(const string fileName);
	static vector<Book*>* findBookISBN(string ISBN);
	static vector<Book*>* findBookTitle(string title);
	static vector<Book*>* findBookAuthor(string author);
	static vector<Book*>* findBookCategory(string category);
	static void sortPopularity(vector<Book*>* books);
	static bool popularityOrder(Book* a, Book* b);
	void addCopy(int dayAvailable);
	void deleteCopy(int id);
	void print();
};