#pragma once

#include "Includes.h"
#include "Object.h"

class Item : public Object {
public:
	Item(char * name);
	Item(const Item & x);
	Item & operator = (const Item & x);
	~Item();

	char * getName();
	
	bool operator == (const Item & x) const;
	bool operator != (const Item & x) const;
	
private:
	char * name;
};
