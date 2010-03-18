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

char * Item::getName() {
	return name;
}

bool Item::operator == (const Item & x) const {
	return _stricmp(this->name, x.name) == 0;
}

bool Item::operator != (const Item & x) const {
	return !operator == (x);
}
