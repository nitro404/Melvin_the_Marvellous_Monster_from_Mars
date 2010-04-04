#pragma once

#include "Includes.h"
#include "Variables.h"
#include "SpriteSheet.h"
#include "SpriteSheets.h"
#include "Items.h"
#include "Player.h"
#include "Graph.h"

class Level : public Items {
public:
	Level(const char * fileName, SpriteSheets * externalSpriteSheets, Variables * settings, double & timeElapsed, int windowWidth, int windowHeight, LPDIRECT3DDEVICE9 d3dDevice);
	~Level();

	bool checkCollision(D3DXVECTOR2 & lastPosition, D3DXVECTOR2 & newPosition, D3DXVECTOR2 & intersection);

	void tick();
	void draw(LPDIRECT3DDEVICE9 d3dDevice);

private:
	void readFrom(ifstream & in);

public:
	Player * player;

private:
	char * name;
	SpriteSheets * spriteSheets;

	Graph boundaries;

	Vertex playerSpawn;
	Vertex petSpawn;
	vector<Object *> objects;
	vector<Object *> tiles;
	vector<Object *> ai;

	double & timeElapsed;
	int windowWidth;
	int windowHeight;
};
