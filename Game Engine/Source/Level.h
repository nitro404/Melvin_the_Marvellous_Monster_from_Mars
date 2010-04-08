#pragma once

#include "Includes.h"
#include "Variables.h"
#include "SpriteSheet.h"
#include "SpriteSheets.h"
#include "Player.h"
#include "Pet.h"
#include "AI.h"
#include "Graph.h"

#if _DEBUG
extern int externalScrollingOffset;
#endif

class Level {
public:
	Level(const char * fileName, SpriteSheets * externalSpriteSheets, Variables * settings, double & timeElapsed, int windowWidth, int windowHeight, LPDIRECT3DDEVICE9 d3dDevice);
	~Level();

	bool checkCollision(const D3DXVECTOR2 & lastPosition, const D3DXVECTOR2 & newPosition, D3DXVECTOR2 * intersection, double * newY);

	void tick();
	void draw(LPDIRECT3DDEVICE9 d3dDevice);

private:
	void readFrom(ifstream & in);

public:
	Player * player;
	Pet * pet;
	vector<Object *> objects;
	vector<Object *> tiles;
	vector<AI *> ai;
	vector<Object *> items;

	int xDimension;
	int yDimension;
	int mapWidth;
	int mapHeight;

private:
	char * name;
	SpriteSheets * spriteSheets;

	Graph boundaries;

	Vertex playerSpawn;
	Vertex petSpawn;

	double & timeElapsed;
	int windowWidth;
	int windowHeight;
	int scrollingOffset;
	LPDIRECT3DDEVICE9 d3dDevice;
};
