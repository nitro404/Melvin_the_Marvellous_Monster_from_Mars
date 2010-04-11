// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

//Created: January 15, 2009
//Revised: February 27, 2010

#pragma once

#include "Variable.h"

using namespace std;

// the variables class represents a collection of variables, which are pairings of identifiers with values
// typically used to read and store configuration settings for an application
class Variables {
public:
	Variables();
	Variables(const Variables & x);
	Variables & operator = (const Variables & x);
	~Variables(void);
	
	bool add(Variable * x); // add a variable
	bool addCopy(const Variable * x); // add a copy of a variable
	Variable * remove(int _index); // remove a variable at a specified index
	Variable * remove(const Variable * x); // remove a variable
	void clear(bool deleteItems); // clear the variables collection
	
	int size() const; // get the number of variables in the collection
	bool contains(const Variable * x) const; // check if a variable is contained in the collection of variables
	int indexOf(const Variable * x) const; // get the index of a variable within the collection of variables
	Variable * elementAt(int _index) const; // get a variable at a specified index
	Variable * getVariable(char * _name) const; // get a variable based on its id
	char * getValue(const char * _variableName); // get the value of a variable based on its id
	bool hasValue(const char * _variableName); // check to see if a variable with a matching id exists in the collection of variables

	bool parseFrom(const char * _fileName, bool _append = false); // parse a variable collection from a file and return it
	
	bool operator == (const Variables & x) const; // equality operator override
	bool operator != (const Variables & x) const; // inequality operator override
	
	void printOn(ostream & o) const; // prints the collection of variables to an output stream
	
private:
	vector<Variable *> * _variables; // the collection of variables
};

ostream & operator << (ostream & o, const Variables & x);
