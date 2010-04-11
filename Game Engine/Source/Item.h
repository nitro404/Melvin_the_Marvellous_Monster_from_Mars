// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#include "Includes.h"
#include "Object.h"

// the item class represents an item which the player can interact with
class Item : public Object {
public:
	Item(char * name);
	Item(const Item & x);
	Item & operator = (const Item & x);
	~Item();

	char * getName(); // returns the name of the item
	
	// equality operator overrides
	bool operator == (const Item & x) const;
	bool operator != (const Item & x) const;
	
private:
	char * name;
};
