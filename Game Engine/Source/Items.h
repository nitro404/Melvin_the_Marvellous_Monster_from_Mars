#pragma once

#include "Item.h"

class Items {
public:
	Items();
	Items(const Items & x);
	Items & operator = (const Items & x);
	~Items();

	bool addItem(Item &item);
	bool removeItem(const Item &item, bool deleteItem);
	bool removeItem(const char * itemName, bool deleteItem);
	bool removeItem(int itemIndex, bool deleteItem);
	Item * getItem(const char * itemName);
	Item * getItem(int itemIndex);
	bool hasItem(Item &item);
	bool hasItem(const char * itemName);
	int itemIndex(const Item &item);
	int itemIndex(const char * itemName);

	bool operator == (const Items & x) const;
	bool operator != (const Items & x) const;

private:
	vector<Item *> items;
};
