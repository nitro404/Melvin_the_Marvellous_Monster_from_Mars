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

int SpriteSheets::size() {
	return spriteSheets.size();
}

SpriteSheet * SpriteSheets::elementAt(int index) {
	return getSpriteSheet(index);
}

SpriteSheet * SpriteSheets::getSpriteSheet(int index) {
	if(index < 0 || index >= (int) spriteSheets.size()) { return NULL; }

	return spriteSheets.at(index);
}

SpriteSheet * SpriteSheets::getSpriteSheet(char * name) {
	if(name == NULL) { return NULL; }
	char * temp = strtrimcpy(name);

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

int SpriteSheets::getSpriteSheetIndex(char * name) {
	if(name == NULL) { return -1; }
	char * temp = strtrimcpy(name);

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

Sprite * SpriteSheets::getSprite(char * name) {
	if(name == NULL) { return NULL; }
	char * temp = strtrimcpy(name);

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

bool SpriteSheets::add(SpriteSheet * spriteSheet) {
	if(spriteSheet == NULL) { return false; }

	for(unsigned int i=0;i<spriteSheets.size();i++) {
		if(spriteSheets.at(i) == spriteSheet) {
			return false;
		}
	}

	spriteSheets.push_back(spriteSheet);
	return true;
}

SpriteSheets * SpriteSheets::parseFrom(char * fileName, char * spriteDirectory, LPDIRECT3DDEVICE9 d3dDevice) {
	if(fileName == NULL || spriteDirectory == NULL) {
		prompt("ERROR: Cannot parse sprite sheet collection from null parameter(s).\n");
		return NULL;
	}

	char * fileNameTrimmed = strtrimcpy(fileName);
	char * spriteDirectoryTrimmed = strtrimcpy(spriteDirectory);

	SpriteSheet * spriteSheet;
	SpriteSheets * spriteSheets = new SpriteSheets();
	ifstream in(fileNameTrimmed);
	if(in.bad()) {
		prompt("ERROR: Unable to read sprite sheet data file.\n");
		delete [] spriteDirectoryTrimmed;
		delete [] fileNameTrimmed;
		return NULL;
	}
	do {
		spriteSheet = SpriteSheet::parseFrom(in, spriteDirectory, d3dDevice);
		if(spriteSheet != NULL) {
			spriteSheets->add(spriteSheet);
		}
	} while(spriteSheet != NULL);
	in.close();

	delete [] fileNameTrimmed;
	delete [] spriteDirectoryTrimmed;

	return spriteSheets;
}
