#include "Reader.h"

int Reader::getMaxCopies() {
	return maxCopies;
}

int Reader::getMaxPeriod() {
	return maxPeriod;
}

vector<Copy*>* Reader::getBorrowedCopies() {
	return &borrowedCopies;
}

vector<Copy*>* Reader::getReservedCopies() {
	return &reservedCopies;
}

int Reader::getPenalties() {
	return penalties;
}

void Reader::setMaxCopies(int maxCopies) {
	this->maxCopies = maxCopies;
}

void Reader::setMaxPeriods(int maxPeriod) {
	this->maxPeriod = maxPeriod;
}

void Reader::setBorrowedCopies(vector<Copy*>* borrowedCopies) {
	this->borrowedCopies = *borrowedCopies;
}

void Reader::setReservedCopies(vector<Copy*>* reservedCopies) {
	this->reservedCopies = *reservedCopies;
}

void Reader::setPenalties(int penalties) {
	this->penalties = penalties;
}

void Reader::borrowCopy(int day) {
	int id;
	cout << "Enter the ID of the copy: ";
	cin >> id;
	cout << endl;

	Copy* copy = Copy::findCopy(id);
	if (copy == NULL) {
		cout << "Error: Unable to find the desired copy" << endl;
	}
	else {
		if (overdue(day)) {
			cout << "Error: You have overdue copies" << endl;
			return;
		}
		if (borrowedCopies.size() >= maxCopies - penalties / 5) {
			cout << "Error: Already borrowing the maximum number of allowed copies" << endl;
			return;
		}
		if (copy->getReservers()->size() >= 1 && copy->getReservers()->at(0)->getUsername() != user) {
			cout << "Error: Copy is reserved for someone else" << endl;
			return;
		}
		if (copy->getReader() != NULL && copy->getReader()->getUsername() == user) {
			cout << "Error: You are already borrowing this copy" << endl;
			return;
		}
		if (!copy->getAvailable()) {
			cout << "Error: The copy is already being borrowed by someone else" << endl;
			return;
		}
		copy->setReader(this);
		copy->setAvailable(false);
		copy->setStart(day);
		int popularity = (copy->getReservers()->size()) / 20;
		copy->setExp(day + maxPeriod - popularity);
		borrowedCopies.push_back(copy);
		cout << "You are now borrowing \"" << copy->getBook()->getTitle() << "\" (ID: " << copy->getID() << ")" << endl;
	}
}

void Reader::returnCopy(int day) {
	int id;
	cout << "Enter the ID of the copy: ";
	cin >> id;
	cout << endl;

	int index;
	for (index = 0; index < borrowedCopies.size(); index++) {
		if (borrowedCopies[index]->getID() == id) {
			break;
		}
	}
	if (index != borrowedCopies.size()) {
		Copy* copy = borrowedCopies.at(index);
		copy->setReader(NULL);
		copy->setAvailable(true);
		copy->setDayAvailable(day);
		copy->setStart(-1);
		copy->setExp(-1);
		borrowedCopies.erase(borrowedCopies.begin() + index);
		cout << "You have returned \"" << copy->getBook()->getTitle() << "\" (ID: " << copy->getID() << ")" << endl;
	}
	else
		cout << "Error: You are not borrowing a copy with that ID" << endl;
}

void Reader::reserveCopy(int day) {
	int id;
	cout << "Enter the ID of the copy: ";
	cin >> id;
	cout << endl;

	Copy* copy = Copy::findCopy(id);
	if (copy == NULL) {
		cout << "Error: Unable to find the desired copy" << endl;
	}
	else {
		if (overdue(day)) {
			cout << "Error: You have overdue copies" << endl;
			return;
		}
		for (int i = 0; i < copy->getReservers()->size(); i++)
		{
			if (copy->getReservers()->at(i)->getUsername() == user)
			{
				cout << "Error: You are already reserving this copy" << endl;
				return;
			}
		}
		copy->addReserver(this, day);
		reservedCopies.push_back(copy);
		cout << "You are now reserving \"" << copy->getBook()->getTitle() << "\" (ID: " << copy->getID() << ") [Position " << copy->getReservers()->size() << "]" << endl;
	}
}

void Reader::cancelReserveCopy() {
	int id;
	cout << "Enter the ID of the copy: ";
	cin >> id;
	cout << endl;

	int index;
	for (index = 0; index < reservedCopies.size(); index++)
	{
		if (reservedCopies.at(index)->getID() == id)
		{
			break;
		}
	}
	if (index != reservedCopies.size())
	{
		Copy* copy = reservedCopies.at(index);
		copy->deleteReserver(this);
		reservedCopies.erase(reservedCopies.begin() + index);
		cout << "You have have been removed from reservations for \"" << copy->getBook()->getTitle() << "\" (ID: " << copy->getID() << ")" << endl;
	}
	else
		cout << "Error: You are not reserving that copy" << endl;
}

void Reader::renewCopy(int day) {
	int id;
	cout << "Enter the ID of the copy: ";
	cin >> id;
	cout << endl;

	int index;
	for (index = 0; index < borrowedCopies.size(); index++)
	{
		if (borrowedCopies.at(index)->getID() == id)
		{
			break;
		}
	}
	if (index != borrowedCopies.size()) {
		Copy* copy = borrowedCopies.at(index);
		if (!copy->getReservers()->empty()) {
			cout << "Error: Copy has reservations" << endl;
			return;
		}
		if (copy->getExp() <= day) {
			cout << "Error: This copy is overdue. No renewals" << endl;
			return;
		}
		if (overdue(day)) {
			cout << "Error: You have overdue copies" << endl;
			return;
		}
		copy->setStart(day);
		int popularity = (copy->getReservers()->size()) / 20;
		copy->setExp(day + maxPeriod - popularity);
		cout << "You have renewed \"" << copy->getBook()->getTitle() << "\" (ID: " << copy->getID() << ") up to Day " << day + maxPeriod - popularity << endl;
	}
	else
		cout << "Error: You are not borrowing a copy with that ID" << endl;
}

void Reader::updatePenalty(int day) {
	for (int i = 0; i < users.size(); i++) { //loops through all the user objects in static vector<User*> users
		if (dynamic_cast<Reader*>(users[i])) { //checks if the user is a Reader
			Reader* reader = (Reader*)users.at(i);
			int penalties = 0; //initialize penalties to zero for each reader
			for (int o = 0; o < reader->getBorrowedCopies()->size(); o++) { //loops through the Books in borrowedCopies of each reader
				if (reader->getBorrowedCopies()->at(o)->getExp() <= day) { //checks if the Book is overdue
					penalties++; //add a penalty if it is overdue
				}
			}
			reader->setPenalties(penalties); //updates the Student's penalty value
		}
	}
}

bool Reader::overdue(int day) {
	for (int i = 0; i < borrowedCopies.size(); i++) {
		if (borrowedCopies[i]->getExp() <= day)
			return true;
	}
	return false;
}