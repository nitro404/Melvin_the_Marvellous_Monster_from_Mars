#pragma once

#include "Includes.h"
#include "SpriteSheet.h"
#include "SpriteSheets.h"
#include "Items.h"
#include "Graph.h"

class Level : public Items {
public:
	Level(const char * fileName, SpriteSheets * externalSpriteSheets);
	~Level();

	void tick();
	void draw(LPDIRECT3DDEVICE9 d3dDevice);

	void readFrom(ifstream & in);

private:
	char * name;
	SpriteSheets * spriteSheets;

	Graph boundaries;

	Vertex playerSpawn;
	Vertex petSpawn;
	vector<Object *> objects;
	vector<Object *> tiles;
	vector<Object *> ai;
};
