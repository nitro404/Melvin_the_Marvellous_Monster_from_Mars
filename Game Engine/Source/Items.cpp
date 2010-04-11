// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

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

// returns the maximum number of items that can be stored
int Items::getItemLimit() {
	return this->limit;
}

// changes the maximum number of items that can be stored
void Items::setItemLimit(int limit) {
	this->limit = limit;
}

// removes the limit on the number of items that can be stored
void Items::removeItemLimit() {
	this->limit = -1;
}

// returns the number of items in the current item collection
int Items::numberOfItems() {
	return this->items.size();
}

// adds an item to the collection
bool Items::addItem(Item &item) {
	if(this->limit > 0 && (int) this->items.size() >= this->limit) { return false; }

	for(unsigned int i=0;i<this->items.size();i++) {
		if(*this->items.at(i) == item) {
			return false;
		}
	}
	this->items.push_back(&item);
	return true;
}

// returns an item based on its name
Item * Items::getItem(const char * itemName) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(_stricmp(this->items.at(i)->getName(), itemName) == 0) {
			return this->items.at(i);
		}
	}
	return NULL;
}

// returns the item at the specified index
Item * Items::getItem(int itemIndex) {
	if(itemIndex < 0 || itemIndex >= (int) this->items.size()) { return NULL; }

	return this->items.at(itemIndex);
}

// returns true if the collection contains the specified item
bool Items::hasItem(Item &item) {
	for(unsigned int i=0;i<items.size();i++) {
		if(*this->items.at(i) == item) {
			return true;
		}
	}
	return false;
}

// returns true if the collection contains the specified item based on its name
bool Items::hasItem(const char * itemName) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(_stricmp(this->items.at(i)->getName(), itemName) == 0) {
			return true;
		}
	}
	return false;
}

// removes an item from the collection
bool Items::removeItem(const Item &item, bool deleteItem) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(*this->items.at(i) == item) {
			this->removeItem(i, deleteItem);
			return true;
		}
	}
	return false;
}

// removes an item from the collection based on its name
bool Items::removeItem(const char * itemName, bool deleteItem) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(_stricmp(this->items.at(i)->getName(), itemName) == 0) {
			this->removeItem(i, deleteItem);
			return true;
		}
	}
	return false;
}

// removes an item from the collection at the specified index
bool Items::removeItem(int itemIndex, bool deleteItem) {
	if(itemIndex < 0 || itemIndex >= (int) this->items.size()) { return false; }

	if(deleteItem) { delete this->items.at(itemIndex); }
	this->items.erase(this->items.begin() + itemIndex);
	return true;
}

// returns the index of the specified item
int Items::itemIndex(const Item &item) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(*this->items.at(i) == item) {
			return i;
		}
	}
	return -1;
}

// returns the index of the item with the specified name
int Items::itemIndex(const char * itemName) {
	for(unsigned int i=0;i<this->items.size();i++) {
		if(_stricmp(this->items.at(i)->getName(), itemName) == 0) {
			return i;
		}
	}
	return -1;
}

// returns true if two items are equal
bool Items::operator == (const Items & x) const {
	if(this->items.size() != x.items.size()) { return false; }

	for(unsigned int i=0;i<this->items.size();i++) {
		if(*this->items.at(i) != *x.items.at(i)) {
			return false;
		}
	}
	return true;
}

// returns true if two items are not equal
bool Items::operator != (const Items & x) const {
	return !operator == (x);
}
