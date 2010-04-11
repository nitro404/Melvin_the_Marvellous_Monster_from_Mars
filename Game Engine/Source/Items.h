// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#include "Item.h"

// the items class represents a collection of objects which can be subclassed to make the subclassed object
// a container as well as having other functionality (essentially dynamic inventories)
class Items {
public:
	Items();
	Items(const Items & x);
	Items & operator = (const Items & x);
	~Items();

	int getItemLimit(); // returns the maximum number of items that can be stored
	void setItemLimit(int limit); // changes the maximum number of items that can be stored
	void removeItemLimit(); // removes the limit on the number of items that can be stored
	int numberOfItems(); // returns the number of items in the current item collection
	bool addItem(Item &item); // adds an item to the collection
	Item * getItem(const char * itemName); // returns an item based on its name
	Item * getItem(int itemIndex); // returns the item at the specified index
	bool hasItem(Item &item); // returns true if the collection contains the specified item
	bool hasItem(const char * itemName); // returns true if the collection contains the specified item based on its name
	bool removeItem(const Item &item, bool deleteItem); // removes an item from the collection
	bool removeItem(const char * itemName, bool deleteItem); // removes an item from the collection based on its name
	bool removeItem(int itemIndex, bool deleteItem); // removes an item from the collection at the specified index
	int itemIndex(const Item &item); // returns the index of the specified item
	int itemIndex(const char * itemName); // returns the index of the item with the specified name

	// equality operator overrides
	bool operator == (const Items & x) const;
	bool operator != (const Items & x) const;

private:
	vector<Item *> items;
	int limit;
};
