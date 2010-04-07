#include "Level.h"

#if _DEBUG
extern D3DXVECTOR2 playerNewPosition;
extern D3DXVECTOR2 playerLastPosition;
int externalScrollingOffset;
extern D3DXVECTOR2 playerCollisionPointA;
extern D3DXVECTOR2 playerCollisionPointB;
extern D3DXVECTOR2 playerCollisionPosition;
#endif

Level::Level(const char * fileName,
			 SpriteSheets * externalSpriteSheets,
			 Variables * settings,
			 double & externalTimeElapsed,
			 int externalWindowWidth,
			 int externalWindowHeight,
			 LPDIRECT3DDEVICE9 d3dDev)
				: name(NULL),
				  player(NULL),
				  pet(NULL),
				  scrollingOffset(0),
				  xDimension(0),
				  yDimension(0),
				  mapWidth(0),
				  mapHeight(0),
				  spriteSheets(externalSpriteSheets),
				  timeElapsed(externalTimeElapsed),
				  windowWidth(externalWindowWidth),
				  windowHeight(externalWindowHeight),
				  d3dDevice(d3dDev) {

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

	player = new Player(windowWidth / 2.0f, (float) windowHeight, windowWidth, windowHeight, timeElapsed, *this, settings, spriteSheets, d3dDevice);
	pet = new Pet(windowWidth / 2.0f, (float) windowHeight, windowWidth, windowHeight, *player, timeElapsed, settings, spriteSheets, d3dDevice);

	this->readFrom(in);

	if(in.is_open()) { in.close(); }
}

Level::~Level() {
	delete [] name;
	delete player;
	if(pet != NULL) { delete pet; }
	for(unsigned int i=0;i<objects.size();i++) {
		delete objects.at(i);
	}
	for(unsigned int i=0;i<tiles.size();i++) {
		delete tiles.at(i);
	}
	for(unsigned int i=0;i<ai.size();i++) {
		delete ai.at(i);
	}
	for(unsigned int i=0;i<items.size();i++) {
		delete items.at(i);
	}
}

bool Level::checkCollision(D3DXVECTOR2 & lastPosition, D3DXVECTOR2 & newPosition, D3DXVECTOR2 * intersection, double * newY) {
	double tempY;
	double minY;
	bool isColliding = false;
	for(int i=0;i<boundaries.size();i++) {
		if(CollisionHandler::checkLineIntersection(*boundaries.elementAt(i), lastPosition, newPosition, intersection, &tempY)) {
			if(!isColliding) {
				minY = tempY;
			}
			else {
// Perhaps check if two points form angle < 180, if so then extend each x coordinate?
				if(tempY < minY) {
					minY = tempY;
				}
			}
			isColliding = true;
		}
	}
	if(newY != NULL && isColliding) {
		*newY = minY;
	}
	return isColliding;
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
	for(unsigned int i=0;i<items.size();i++) {
		items.at(i)->tick();
	}
	player->tick();
	if(pet != NULL) { pet->tick(); }

	int halfWidth = (windowWidth / 2) - 1;
	float xPos = player->getCenter().x;
	if(xPos > halfWidth && xPos < (mapWidth - 1) - halfWidth) {
		scrollingOffset = (int) (player->getCenter().x - halfWidth);
	}
#if _DEBUG
	externalScrollingOffset = scrollingOffset;
#endif
}

void Level::draw(LPDIRECT3DDEVICE9 d3dDevice) {
	for(unsigned int i=0;i<tiles.size();i++) {
		tiles.at(i)->draw(&scrollingOffset, d3dDevice);
	}
	for(unsigned int i=0;i<objects.size();i++) {
		objects.at(i)->draw(&scrollingOffset, d3dDevice);
	}
	for(unsigned int i=0;i<ai.size();i++) {
		ai.at(i)->draw(&scrollingOffset, d3dDevice);
	}
	player->draw(&scrollingOffset, d3dDevice);
	if(pet != NULL) { pet->draw(&scrollingOffset, d3dDevice); }
	for(unsigned int i=0;i<items.size();i++) {
		items.at(i)->draw(&scrollingOffset, d3dDevice);
	}

#if _DEBUG
	testDrawPoint(d3dDevice, (float) playerLastPosition.x, (float) playerLastPosition.y, D3DCOLOR_XRGB(255, 0, 255), &externalScrollingOffset);
	testDrawPoint(d3dDevice, (float) playerNewPosition.x, (float) playerNewPosition.y, D3DCOLOR_XRGB(255, 0, 255), &externalScrollingOffset);

	testDrawLine(d3dDevice, (float) playerLastPosition.x, (float) playerLastPosition.y,
							(float) playerNewPosition.x, (float) playerNewPosition.y, D3DCOLOR_XRGB(255, 0, 255), &externalScrollingOffset);

	for(int i=0;i<boundaries.size();i++) {
		boundaries.elementAt(i)->draw(d3dDevice);

		if(CollisionHandler::checkLineIntersection(*boundaries.elementAt(i), playerLastPosition, playerNewPosition, NULL, NULL)) {
			testDrawBox(d3dDevice, 40, 40, 70, 70, D3DCOLOR_XRGB(255, 255, 255), &externalScrollingOffset);
		}
	}

	testDrawLine(d3dDevice, playerCollisionPointA.x, playerCollisionPointA.y, playerCollisionPointB.x, playerCollisionPointB.y, D3DCOLOR_XRGB(255, 0, 0), &externalScrollingOffset);
	testDrawPoint(d3dDevice, playerCollisionPosition.x, playerCollisionPosition.y, D3DCOLOR_XRGB(255, 0, 0), &externalScrollingOffset);
#endif
}

void Level::readFrom(ifstream &in) {
	const int maxLength = 256;
	char input[maxLength];
	char * temp, * temp2, * xData, * yData;

	in.getline(input, maxLength); // header

	in.getline(input, maxLength); // grid size

	in.getline(input, maxLength); // dimension
	temp = strtrimcpy(input);
	xData = strchr(temp, ':') + sizeof(char);
	yData = strchr(xData, ',');
	*yData = '\0';
	yData += sizeof(char);
	xDimension = atoi(xData);
	yDimension = atoi(yData);
	delete [] temp;
	mapWidth = xDimension * Constants::GRID_SIZE;
	mapHeight = yDimension * Constants::GRID_SIZE;

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
	player->setPosition((float) playerSpawn.x, (float) playerSpawn.y);

	in.getline(input, maxLength); // pet
	temp = strtrimcpy(input);
	xData = strchr(temp, ':') + sizeof(char);
	temp2 = strtrimcpy(xData);
	if(_stricmp(temp2, "None") == 0) {
		delete [] temp2;
		delete [] temp;
		if(pet != NULL) { delete pet; }
		pet = NULL;
	}
	else {
		delete [] temp2;
		yData = strchr(xData, ',');
		*yData = '\0';
		yData += sizeof(char);
		*strchr(yData, ',') = '\0';
		petSpawn = Vertex(atoi(xData), atoi(yData));
		delete [] temp;
		pet->setPosition((float) petSpawn.x, (float) petSpawn.y);
	}

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

	in.getline(input, maxLength); // items
	int numberOfItems = atoi(strchr(input, ':') + sizeof(char));
	for(int i=0;i<numberOfItems;i++) {
		in.getline(input, maxLength);
		Object * newItem = Object::parseFrom(input, *spriteSheets);
		if(newItem != NULL) { items.push_back(newItem); }
	}
}
