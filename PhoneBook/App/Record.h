#pragma once
#pragma once
#pragma once
#pragma once
#include <string>
using namespace std;

class Record {
public:
	string phoneNumber;
	string name;
	string surname;
	string fathersName;
	string street;
	int houseNumber;
	int blockNumber;
	int flatNumber;

	Record(string _phoneNumber, string _surname, string _name, string _fathersName, string _street, int _houseNumber, int _blockNumber, int _flatNumber) {
		phoneNumber = _phoneNumber;
		name = _name;
		surname = _surname;
		fathersName = _fathersName;
		street = _street;
		houseNumber = _houseNumber;
		blockNumber = _blockNumber;
		flatNumber = _flatNumber;
	}

	Record() {}
};

