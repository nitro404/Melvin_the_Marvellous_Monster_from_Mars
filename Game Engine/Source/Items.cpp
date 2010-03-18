#include "Items.h"

Items::Items() { }

Items::Items(const Items & x) {
	for(unsigned int i=0;i<x.items.size();i++) {
		this->items.push_back(new Item(*x.items.at(i)));
	}
}

Items & Items::operator = (const Items & x) {
	for(unsigned int i=0;i<this->items.size();i++) {
		delete this->items.at(i);
	}
	this->items.clear();
	
	for(unsigned int i=0;i<x.items.size();i++) {
		this->items.push_back(new Item(*x.items.at(i)));
	}
	
	return *this;
}

Items::~Items() {
	for(unsigned int i=0;i<this->items.size();i++) {
		delete this->items.at(i);
	}
}

bool Items::addItem(Item &item) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(*this->items.at(i) == item) {
			return false;
		}
	}
	this->items.push_back(&item);
	return true;
}

bool Items::removeItem(const Item &item, bool deleteItem) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(*this->items.at(i) == item) {
			this->removeItem(i, deleteItem);
			return true;
		}
	}
	return false;
}

bool Items::removeItem(const char * itemName, bool deleteItem) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(_stricmp(this->items.at(i)->getName(), itemName) == 0) {
			this->removeItem(i, deleteItem);
			return true;
		}
	}
	return false;
}

bool Items::removeItem(int itemIndex, bool deleteItem) {
	if(itemIndex < 0 || itemIndex >= (int) this->items.size()) { return false; }

	if(deleteItem) { delete this->items.at(itemIndex); }
	this->items.erase(this->items.begin() + itemIndex);
}

Item * Items::getItem(const char * itemName) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(_stricmp(this->items.at(i)->getName(), itemName) == 0) {
			return this->items.at(i);
		}
	}
	return NULL;
}

Item * Items::getItem(int itemIndex) {
	if(itemIndex < 0 || itemIndex >= (int) this->items.size()) { return NULL; }

	return this->items.at(itemIndex);
}

bool Items::hasItem(Item &item) {
	for(unsigned int i=0;i<items.size();i++) {
		if(*this->items.at(i) == item) {
			return true;
		}
	}
	return false;
}

bool Items::hasItem(const char * itemName) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(_stricmp(this->items.at(i)->getName(), itemName) == 0) {
			return true;
		}
	}
	return false;
}

int Items::itemIndex(const Item &item) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(*this->items.at(i) == item) {
			return i;
		}
	}
	return -1;
}

int Items::itemIndex(const char * itemName) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(_stricmp(this->items.at(i)->getName(), itemName) == 0) {
			return i;
		}
	}
	return -1;
}

bool Items::operator == (const Items & x) const {
	if(this->items.size() != x.items.size()) { return false; }

	for(unsigned int i=0;i<this->items.size();i++) {
		if(*this->items.at(i) != *x.items.at(i)) {
			return false;
		}
	}
	return true;
}

bool Items::operator != (const Items & x) const {
	return !operator == (x);
}
