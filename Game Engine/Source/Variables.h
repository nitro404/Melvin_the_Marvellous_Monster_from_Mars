//Created: January 15, 2009
//Revised: February 27, 2010

#ifndef _VARIABLES_H
#define _VARIABLES_H

#include "Variable.h"

using namespace std;

#pragma warning(disable: 4786)

class Variables {
public:
	Variables();
	Variables(const Variables & x);
	Variables & operator = (const Variables & x);
	~Variables(void);
	
	bool add(const Variable * x);
	bool remove(int _index);
	bool remove(const Variable * x);
	void clear();
	
	int size() const;
	bool contains(const Variable * x) const;
	int indexOf(const Variable * x) const;
	Variable * elementAt(int _index) const;
	char * getValue(const char * _variableName);
	bool hasValue(const char * _variableName);

	bool parseFrom(const char * _fileName, bool _append = false);
	
	bool operator == (const Variables & x) const;
	bool operator != (const Variables & x) const;
	
	void printOn(ostream & o) const;
	
private:
	vector<Variable *> * _variables;
};

ostream & operator << (ostream & o, const Variables & x);

#endif