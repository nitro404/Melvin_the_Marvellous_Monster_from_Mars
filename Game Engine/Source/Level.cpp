#include "Level.h"

Level::Level(const char * fileName,
			 SpriteSheets * externalSpriteSheets,
			 Variables * settings,
			 double & externalTimeElapsed,
			 int externalWindowWidth,
			 int externalWindowHeight,
			 LPDIRECT3DDEVICE9 d3dDevice)
				: name(NULL),
				  timeElapsed(externalTimeElapsed),
				  spriteSheets(externalSpriteSheets),
				  windowWidth(externalWindowWidth),
				  windowHeight(externalWindowHeight) {

	if(spriteSheets == NULL) {
		quit("Error", "Cannot open level without corresponding sprite sheet collection.");
	}

	ifstream in;
	in.open(fileName); 
	if(in.bad()) {
		quit("Error", "Unable to open level: \"%s\".", fileName);
	}

	const char * levelNameWithExt = strchr(fileName, '\\') + sizeof(char);
	char * tempLevelName = new char[strlen(levelNameWithExt) + 1];
	strcpy_s(tempLevelName, strlen(levelNameWithExt) + 1, levelNameWithExt);
	char * levelExt = strrchr(tempLevelName, '.');
	*levelExt = '\0';
	this->name = new char[strlen(tempLevelName) + 1];
	strcpy_s(this->name, strlen(tempLevelName) + 1, tempLevelName);
	delete [] tempLevelName;

	player = new Player(windowWidth / 2.0f, (float) windowHeight, windowWidth, windowHeight, timeElapsed, settings, spriteSheets, d3dDevice);

	this->readFrom(in);

	if(in.is_open()) { in.close(); }
}

Level::~Level() {
	delete [] name;
	delete player;
	for(unsigned int i=0;i<objects.size();i++) {
		delete objects.at(i);
	}
	for(unsigned int i=0;i<tiles.size();i++) {
		delete tiles.at(i);
	}
	for(unsigned int i=0;i<ai.size();i++) {
		delete ai.at(i);
	}
}

bool Level::checkCollision(D3DXVECTOR2 & lastPosition, D3DXVECTOR2 & newPosition, D3DXVECTOR2 & intersection) {
	for(int i=0;i<boundaries.size();i++) {
		if(CollisionHandler::checkLineIntersection(*boundaries.elementAt(i), lastPosition, newPosition, intersection)) {
			return true;
		}
	}
	return false;
}

void Level::tick() {
	for(unsigned int i=0;i<tiles.size();i++) {
		tiles.at(i)->tick();
	}
	for(unsigned int i=0;i<objects.size();i++) {
		objects.at(i)->tick();
	}
	for(unsigned int i=0;i<ai.size();i++) {
		ai.at(i)->tick();
	}
	player->tick();
}

void Level::draw(LPDIRECT3DDEVICE9 d3dDevice) {
	for(unsigned int i=0;i<tiles.size();i++) {
		tiles.at(i)->draw(d3dDevice);
	}
	for(unsigned int i=0;i<objects.size();i++) {
		objects.at(i)->draw(d3dDevice);
	}
	for(unsigned int i=0;i<ai.size();i++) {
		ai.at(i)->draw(d3dDevice);
	}
	player->draw(d3dDevice);
}

void Level::readFrom(ifstream &in) {
	const int maxLength = 256;
	char input[maxLength];
	char * temp, * xData, * yData;

	in.getline(input, maxLength); // header
	in.getline(input, maxLength); // grid size
	in.getline(input, maxLength); // dimensions

	in.getline(input, maxLength); // edges
	int numberOfEdges = atoi(strchr(input, ':') + sizeof(char));
	for(int i=0;i<numberOfEdges;i++) {
		in.getline(input, maxLength);
		boundaries.addEdge(Edge::parseFrom(input));
	}

	in.getline(input, maxLength); // player
	temp = strtrimcpy(input);
	xData = strchr(temp, ':') + sizeof(char);
	yData = strchr(xData, ',');
	*yData = '\0';
	yData += sizeof(char);
	*strchr(yData, ',') = '\0';
	playerSpawn = Vertex(atoi(xData), atoi(yData));
	delete [] temp;
	player->setPosition((float) playerSpawn.x + player->getOffsetX(), (float) playerSpawn.y + player->getOffsetY());

	in.getline(input, maxLength); // pet
	temp = strtrimcpy(input);
	xData = strchr(temp, ':') + sizeof(char);
	yData = strchr(xData, ',');
	*yData = '\0';
	yData += sizeof(char);
	*strchr(yData, ',') = '\0';
	petSpawn = Vertex(atoi(xData), atoi(yData));
	delete [] temp;
//	pet->setPosition(petSpawn.x, petSpawn.y);

	in.getline(input, maxLength); // objects
	int numberOfObjects = atoi(strchr(input, ':') + sizeof(char));
	for(int i=0;i<numberOfObjects;i++) {
		in.getline(input, maxLength);
		Object * newObject = Object::parseFrom(input, *spriteSheets);
		if(newObject != NULL) { objects.push_back(newObject); }
	}

	in.getline(input, maxLength); // tiles
	int numberOfTiles = atoi(strchr(input, ':') + sizeof(char));
	for(int i=0;i<numberOfTiles;i++) {
		in.getline(input, maxLength);
		Object * newTile = Object::parseFrom(input, *spriteSheets);
		if(newTile != NULL) { tiles.push_back(newTile); }
	}

	in.getline(input, maxLength); // ai
	int numberOfAI = atoi(strchr(input, ':') + sizeof(char));
	for(int i=0;i<numberOfAI;i++) {
		in.getline(input, maxLength);
		Object * newAI = Object::parseFrom(input, *spriteSheets);
		if(newAI != NULL) { ai.push_back(newAI); }
	}
}
