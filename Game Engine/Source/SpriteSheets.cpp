// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#include "SpriteSheets.h"

SpriteSheets::SpriteSheets() {

}

SpriteSheets::~SpriteSheets() {
	for(unsigned int i=0;i<spriteSheets.size();i++) {
		if(spriteSheets.at(i) != NULL) {
			delete spriteSheets.at(i);
		}
	}
}

// return the number of sprite sheets in the sprite sheet collection
int SpriteSheets::size() {
	return spriteSheets.size();
}

// get a sprite sheet at a specific index
SpriteSheet * SpriteSheets::elementAt(int index) {
	return getSpriteSheet(index);
}

// get a sprite sheet at a specific index
SpriteSheet * SpriteSheets::getSpriteSheet(int index) {
	if(index < 0 || index >= (int) spriteSheets.size()) { return NULL; }

	return spriteSheets.at(index);
}

// get a sprite sheet by name
SpriteSheet * SpriteSheets::getSpriteSheet(char * name) {
	if(name == NULL) { return NULL; }
	char * temp = strtrimcpy(name);

	// loop through the collection of sprite sheets and return the sprite sheet with a matching name
	for(unsigned int i=0;i<spriteSheets.size();i++) {
		if(spriteSheets.at(i)->getName() != NULL &&
		   _stricmp(temp, spriteSheets.at(i)->getName()) == 0) {
			delete [] temp;
			return spriteSheets.at(i);
		}
	}

	delete [] temp;
	return NULL;
}

// get the index of a sprite sheet by its name
int SpriteSheets::getSpriteSheetIndex(char * name) {
	if(name == NULL) { return -1; }
	char * temp = strtrimcpy(name);

	// loop through the collection of sprite sheets and return the index of the sprite sheet with a matching name
	for(unsigned int i=0;i<spriteSheets.size();i++) {
		if(spriteSheets.at(i)->getName() != NULL &&
		   _stricmp(temp, spriteSheets.at(i)->getName()) == 0) {
			delete [] temp;
			return i;
		}
	}

	delete [] temp;
	return -1;
}

// get a sprite from any sprite sheet by its name
Sprite * SpriteSheets::getSprite(char * name) {
	if(name == NULL) { return NULL; }
	char * temp = strtrimcpy(name);

	// loop through the collection of sprite sheets and the sprites contained in each sprite sheet
	// until a sprite with a matching name is found, then return it
	for(unsigned int i=0;i<spriteSheets.size();i++) {
		for(int j=0;j<spriteSheets.at(i)->size();j++) {
			if(spriteSheets.at(i)->elementAt(j)->getName() != NULL &&
			   _stricmp(temp, spriteSheets.at(i)->elementAt(j)->getName()) == 0) {
				delete [] temp;
				return spriteSheets.at(i)->elementAt(j);
			}
		}
	}

	delete [] temp;
	return NULL;
}

// add a sprite sheet to the sprite sheet collection
bool SpriteSheets::add(SpriteSheet * spriteSheet) {
	if(spriteSheet == NULL) { return false; }

	// ensure that the sprite sheet is not already contained in the sprite sheet collection
	for(unsigned int i=0;i<spriteSheets.size();i++) {
		if(spriteSheets.at(i) == spriteSheet) {
			return false;
		}
	}

	// if it is not a duplicate, then add it
	spriteSheets.push_back(spriteSheet);
	return true;
}

// parse a collection of sprite sheets from a file
SpriteSheets * SpriteSheets::parseFrom(char * fileName, char * spriteDirectory, LPDIRECT3DDEVICE9 d3dDevice) {
	if(fileName == NULL || spriteDirectory == NULL) {
		prompt("ERROR: Cannot parse sprite sheet collection from null parameter(s).\n");
		return NULL;
	}

	// trim the file name and directory name
	char * fileNameTrimmed = strtrimcpy(fileName);
	char * spriteDirectoryTrimmed = strtrimcpy(spriteDirectory);

	// open the sprite sheet data file
	SpriteSheet * spriteSheet;
	SpriteSheets * spriteSheets = new SpriteSheets();
	ifstream in(fileNameTrimmed);
	if(in.bad()) {
		prompt("ERROR: Unable to read sprite sheet data file.\n");
		delete [] spriteDirectoryTrimmed;
		delete [] fileNameTrimmed;
		return NULL;
	}

	// loop until an invalid sprite sheet is returned or the end of the data file is found
	do {
		// parse a sprite sheet from the current position in the input stream
		spriteSheet = SpriteSheet::parseFrom(in, spriteDirectory, d3dDevice);

		// if the sprite sheet was valid then add it to the sprite sheet collection
		if(spriteSheet != NULL) {
			spriteSheets->add(spriteSheet);
		}
	} while(spriteSheet != NULL);

	if(in.is_open()) { in.close(); }

	delete [] fileNameTrimmed;
	delete [] spriteDirectoryTrimmed;

	return spriteSheets;
}
