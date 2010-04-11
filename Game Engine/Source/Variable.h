// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

//Created: September 18, 2008
//Revised: February 14, 2010

#pragma once

#include "Includes.h"

using namespace std;

class Variable {
public:
	Variable(const char * _id = "", const char * _value = "");
	Variable(const Variable & x);
	Variable & operator = (const Variable & x);
	~Variable(void);
	
	void id(const char * _id); // change the id of the variable
	void value(const char * _value); // change the value of the variable
	
	char * id() const; // get the variable id
	char * value() const; // get the variable value

	bool parseFrom(const char * _data); // parse the variable from a string of the form "ID: Value"
	
	bool operator == (const Variable & x) const; // equality operator override
	bool operator != (const Variable & x) const; // inequality operator override
	
	void printOn(ostream & o) const; // prints the variable to an output stream
	
private:
	char * _id;
	char * _value;
};

ostream & operator << (ostream & o, const Variable & x);
