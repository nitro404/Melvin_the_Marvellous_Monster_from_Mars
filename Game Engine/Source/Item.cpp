// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#include "Item.h"

Item::Item(char * name) : name(0) {
	if(name == NULL) {
		this->name = new char[1];
		this->name[0] = '\0';
	}
	else {
		this->name = new char[strlen(name) + 1];
		strcpy_s(this->name, strlen(name) + 1, name);
	}

	position = D3DXVECTOR2(0, 0);
	offset = D3DXVECTOR2(0, 0);
	orientation = 0;
	velocity = D3DXVECTOR2(0, 0);
	scale = D3DXVECTOR2(1, 1);
}

Item::Item(const Item & x) {
	this->name = new char[strlen(x.name) + 1];
	strcpy_s(this->name, strlen(x.name) + 1, x.name);
}

Item & Item::operator = (const Item & x) {
	delete [] this->name;

	this->name = new char[strlen(x.name) + 1];
	strcpy_s(this->name, strlen(x.name) + 1, x.name);

	return *this;
}

Item::~Item() {
	delete [] name;
}

// returns the name of the item
char * Item::getName() {
	return name;
}

// returns true of two items are equal
bool Item::operator == (const Item & x) const {
	return _stricmp(this->name, x.name) == 0;
}

// returns true if two items are not equal
bool Item::operator != (const Item & x) const {
	return !operator == (x);
}
