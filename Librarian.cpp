#include "Librarian.h"
#include "Student.h"
#include "Teacher.h"

Librarian::Librarian() {
	user = "";
	pass = "";
}

Librarian::Librarian(string user, string pass) {
	this->user = user;
	this->pass = pass;
}

void Librarian::operator<<(const string fileName) {
	ifstream in(fileName); //opens librarian.txt
	if (in.fail()) {
		cout << "Could not open librarian.txt" << endl; //program terminates if file is unable to be opened
		exit(1);
	}

	string user, pass;
	while (in >> user >> pass) { //reads in the data for each librarian from the file
		users.push_back(new Librarian(user, pass));//adds the librarian into static vector<User*> users
	}

	in.close(); //close ifstream
}

void Librarian::operator>>(const string fileName) {
	ofstream out(fileName, ofstream::out | ofstream::trunc); //clears and opens the output file
	if (out.fail()) {
		cout << "Could not open librarian.txt" << endl; //program terminates if file is unable to be opened
		exit(1);
	}

	for (int i = 0; i < users.size(); i++) { //loop through all users in the system
		if (typeid(*users[i]) == typeid(Librarian)) { //write data if the user is a librarian
			out << users[i]->getUsername() << " " << users[i]->getPassword() << endl;
		}
	}

	out.close(); //close ofstream
}

void Librarian::addBook(int day) {
	string ISBN, title, author, category, index;
	vector<Book*>* books;

	cout << "Enter the ISBN of the book: ";
	cin >> ISBN;
	cout << "Enter the title of the book: ";
	cin >> title;
	cout << "Enter the author of the book: ";
	cin >> author;
	cout << "Enter the category of the book: ";
	cin >> category;
	cout << "Enter the index of the book: ";
	cin >> index;
	cout << endl;

	books = Book::findBookISBN(ISBN);

	if (books == NULL) {
		cout << "A new book \"" << title << "\" has been added into the system" << endl;
		Book* book = new Book(ISBN, title, author, category, index);
		book->addCopy(day);
		Book::books.push_back(book);
		return;
	}
	books->front()->addCopy(day);
}

void Librarian::addUser()
{
	string user, pass;
	int type;

	cout << "Enter the username: ";
	cin >> user;
	cout << "Enter the password: ";
	cin >> pass;
	cout << "Enter the type of the new user (0 = librarian, 1 = student, 2 = teacher): ";
	cin >> type;
	cout << endl;

	if (usernameTaken(user)) {
		cout << "Error: That username is already taken" << endl;
		return;
	}

	User* new_user;
	switch (type)
	{
	case 0:
		new_user = new Librarian(user, pass);
		break;
	case 1:
		new_user = new Student(user, pass);
		break;
	case 2:
		new_user = new Teacher(user, pass);
		break;
	default:
		cout << "Error: That is not a valid type" << endl;
		return;
	}

	users.push_back(new_user);
	cout << "Successfully created a " << (type == 0 ? "librarian" : (type == 1 ? "student" : "teacher")) << " with the username \"" << user << "\"" << endl;
}

void Librarian::deleteBook() {
	int id;
	cout << "Enter the ID of the copy: ";
	cin >> id;
	cout << endl;

	Copy* copy = Copy::findCopy(id);

	if (copy == NULL) {
		cout << "Error: Copy does not exist" << endl;
		return;
	}
	if (copy->getReader() != NULL) {
		cout << "Error: Copy is being borrowed" << endl;
		return;
	}

	Book* book = copy->getBook();
	book->deleteCopy(id);
}

void Librarian::deleteUser()
{
	string user;
	cout << "Enter the target's username: ";
	cin >> user;
	cout << endl;

	int index;
	for (index = 0; index < users.size(); index++)
	{
		if (users.at(index)->getUsername() == user)
		{
			break;
		}
	}

	if (index == users.size())
	{
		cout << "Error: User does not exist" << endl;
		return;
	}
	else
	{
		if (users.at(index)->getUsername() == this->user) {
			cout << "Error: Can not delete your own account" << endl;
			return;
		}
		if (typeid(*users.at(index)) == typeid(Student) || typeid(*users.at(index)) == typeid(Teacher))
		{
			Reader* reader = (Reader*)users.at(index);
			if (!reader->getBorrowedCopies()->empty())
			{
				cout << "Error: User is still borrowing book(s)" << endl;
				return;
			}
			for (int i = 0; i < reader->getReservedCopies()->size(); i++)
			{
				reader->getReservedCopies()->at(i)->deleteReserver(reader);
			}
		}
		cout << "The " <<
			(typeid(*users.at(index)) == typeid(Librarian) ? "librarian" : (typeid(*users.at(index)) == typeid(Student) ? "student" : "teacher"))
			<< " with the username \"" << user << "\" has been removed from the system" << endl;
		users.erase(users.begin() + index);
	}
}

void Librarian::searchUser() {
	string username;
	cout << "Enter the target's username: ";
	cin >> username;
	cout << endl;

	User* user = findUser(username);
	if (user != NULL) {
		user->print();
	}
	else {
		cout << "Error: User does not exist" << endl;
	}
}

void Librarian::print() {
	cout << "Username: " << user << endl;
	cout << "Password: " << pass << endl;
}