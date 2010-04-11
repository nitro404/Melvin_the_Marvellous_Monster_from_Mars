// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

//Created: January 15, 2009
//Revised: April 10, 2010

#include "Variables.h"

Variables::Variables() {
	this->_variables = new vector<Variable *>;
}

Variables::Variables(const Variables & x) {
	this->_variables = new vector<Variable *>;
	for(int i=0;i<x.size();i++) {
		this->_variables->push_back( new Variable( *(x.elementAt(i)) ) );
	}
}

Variables & Variables::operator = (const Variables & x) {
	for(unsigned int i=0;i<this->_variables->size();i++) {
		delete this->_variables->at(i);
	}
	delete this->_variables;

	this->_variables = new vector<Variable *>;
	for(int i=0;i<x.size();i++) {
		this->_variables->push_back( new Variable( *(x.elementAt(i)) ) );
	}

	return *this;
}

Variables::~Variables(void) {
	for(unsigned int i=0;i<this->_variables->size();i++) {
		delete this->_variables->at(i);
	}
	delete this->_variables;
}

// add a variable
bool Variables::add(Variable * x) {
	if(x == NULL || this->contains(x)) {
		return false;
	}
	
	this->_variables->push_back(x);
	
	return true;
}

// add a copy of a variable
bool Variables::addCopy(const Variable * x) {
	if(x == NULL || this->contains(x)) {
		return false;
	}
	
	this->_variables->push_back( new Variable(*(x)) );
	
	return true;
}

// remove a variable at a specified index
Variable * Variables::remove(int _index) {
	if(_index < 0 || _index >= size()) {
		return NULL;
	}
	
	Variable * v = this->_variables->at(_index);
	this->_variables->erase(this->_variables->begin() + _index);
	
	return v;
}

// remove a variable
Variable * Variables::remove(const Variable * x) {
	if(x == NULL) {
		return NULL;
	}

	int index = this->indexOf(x);
	if(index < 0) {
		return false;
	}

	Variable * v = this->_variables->at(index);
	this->_variables->erase(this->_variables->begin() + index);
	
	return v;
}

// clear the variables collection
void Variables::clear(bool deleteItems) {
	if(deleteItems && this->_variables->size() > 0) {
		for(unsigned int i=0;i<this->_variables->size();i++) {
			delete this->_variables->at(i);
		}
	}
	this->_variables->clear();
}

// get the number of variables in the collection
int Variables::size() const {
	return this->_variables->size();
}

// check if a variable is contained in the collection of variables
bool Variables::contains(const Variable * x) const {
	if(x == NULL) { return false; }

	for(int i=0;i<size();i++) {
		if( *(this->elementAt(i)) == *(x) ) {
			return true;
		}
	}
	return false;
}

// get the index of a variable within the collection of variables
int Variables::indexOf(const Variable * x) const {
	if(x == NULL) {
		return -1;
	}

	for(int i=0;i<size();i++) {
		if( *(this->elementAt(i)) == *(x) ) {
			return i;
		}
	}
	return -1;
}

// get a variable at a specified index
Variable * Variables::elementAt(int _index) const {
	if(_index < 0 || _index >= size()) {
		return NULL;
	}

	return this->_variables->at(_index);
}

// get a variable based on its id
Variable * Variables::getVariable(char * _name) const {
	if(_name == NULL || strlen(_name) == 0) { return NULL; }

	for(int i=0;i<size();i++) {
		if( _stricmp(this->elementAt(i)->id(), _name) == 0 ) {
			return this->elementAt(i);
		}
	}
	return NULL;
}

// get the value of a variable based on its id
char * Variables::getValue(const char * _variableName) {
	if(_variableName == NULL || strlen(_variableName) == 0) {
		return NULL;
	}

	for(int i=0;i<size();i++) {
		if( _stricmp(this->elementAt(i)->id(), _variableName) == 0 ) {
			return this->elementAt(i)->value();
		}
	}
	return NULL;
}

// check to see if a variable with a matching id exists in the collection of variables
bool Variables::hasValue(const char * _variableName) {
	if(_variableName == NULL || strlen(_variableName) == 0) {
		return false;
	}

	for(int i=0;i<size();i++) {
		if( _stricmp(this->elementAt(i)->id(), _variableName) == 0 ) {
			return true;
		}
	}
	return false;
}

// parse a variable collection from a file and return it
bool Variables::parseFrom(const char * _fileName, bool _append) {
	if(_fileName == NULL || strlen(_fileName) == 0) {
		return false;
	}
	
	// declare input variables
	const int MAX_STRING_LENGTH = 1024;
	unsigned int i;
	char temp[MAX_STRING_LENGTH];
	Variable * v;
	bool duplicate;
	
	// open the file
	ifstream fpt(_fileName);
	if(fpt == NULL) {
		return false;
	}
	
	// if appending to the current collection and the current collection does not exist, initialise it
	// otherwise delete the current collection and re-initialise it
	if(_append) {
		if(this->_variables == NULL) {
			this->_variables = new vector<Variable *>;
		}
	}
	else {
		if(this->_variables != NULL) {
			delete this->_variables;
		}
		this->_variables = new vector<Variable *>;
	}
	
	// loop through the entire file
	while(!fpt.eof()) {
		// input a line and parse a variable from it
		fpt.getline(temp, MAX_STRING_LENGTH);
		v = new Variable();
		if(v->parseFrom(temp)) {
			// if a variable was successfully parsed, check to see that it is not a duplicate
			duplicate = false;
			for(i=0;i<this->_variables->size();i++) {
				if(*this->_variables->at(i) == *v) {
					duplicate = true;
					delete v;
					break;
				}
			}
			// if the variable is not a duplicate, add the variable
			if(!duplicate) {
				this->_variables->push_back(v);
			}
		}
		// if the variable was unsuccessfully parsed, delete it
		else {
			delete v;
		}
	}
	
	return true;
}

// equality operator override
bool Variables::operator == (const Variables & x) const {
	bool contains;
	for(unsigned int i=0;i<this->_variables->size();i++) {
		contains = false;
		for(int j=0;j<x.size();j++) {
			if( *(this->_variables->at(i)) == *(x._variables->at(j)) ) {
				contains = true;
				break;
			}
		}
		if(!contains) {
			return false;
		}
	}
	return true;
}

// inequality operator override
bool Variables::operator != (const Variables & x) const {
	return !operator == (x);
}

// prints the collection of variables to an output stream
void Variables::printOn(ostream & o) const {
	for(unsigned int i=0;i<this->_variables->size();i++) {
		o << this->_variables->at(i)->id() << ": " << this->_variables->at(i)->value();
		
		if(i < this->_variables->size() - 1) {
			o << endl;
		}
	}
}

ostream & operator << (ostream & o, const Variables & x) {
	x.printOn(o);
	return o;
}
