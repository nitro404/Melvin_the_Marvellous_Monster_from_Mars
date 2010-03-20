#include "Level.h"

Level::Level() {

}

Level::Level(char * fileName) {
	ifstream in;
	in.open(fileName); 
	if(in.bad()) {
		quit("ERROR: Unable to open level: \"%s\".", fileName);
	}

	this->readFrom(in);
}

Level::~Level() {
	for(unsigned int i=0;i<levelSpriteSheets.size();i++) {
		delete levelSpriteSheets.at(i);
	}
	for(unsigned int i=0;i<levelSpriteSheetImages.size();i++) {
		delete levelSpriteSheetImages.at(i);
	}
}

void Level::tick() {
	
}

void Level::draw(LPDIRECT3DDEVICE9 d3dDevice) {
	
}

void Level::readFrom(ifstream &in) {
	char input[256];

	in.getline(input, 256, '\n');
}