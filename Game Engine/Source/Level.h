#pragma once

#include "Includes.h"
#include "Variables.h"
#include "SpriteSheet.h"
#include "SpriteSheets.h"
#include "Player.h"
#include "Pet.h"
#include "Graph.h"

class Level {
public:
	Level(const char * fileName, SpriteSheets * externalSpriteSheets, Variables * settings, double & timeElapsed, int windowWidth, int windowHeight, LPDIRECT3DDEVICE9 d3dDevice);
	~Level();

//	bool checkCollision(D3DXVECTOR2 & position, double radius);
	bool checkCollision(D3DXVECTOR2 & lastPosition, D3DXVECTOR2 & newPosition, D3DXVECTOR2 & intersection);

	void tick();
	void draw(LPDIRECT3DDEVICE9 d3dDevice);

private:
	void readFrom(ifstream & in);

public:
	Player * player;
	Pet * pet;
	vector<Object *> objects;
	vector<Object *> tiles;
	vector<Object *> ai;
	vector<Object *> items;

private:
	char * name;
	SpriteSheets * spriteSheets;

	Graph boundaries;

	Vertex playerSpawn;
	Vertex petSpawn;

	double & timeElapsed;
	int windowWidth;
	int windowHeight;
	LPDIRECT3DDEVICE9 d3dDevice;
};
