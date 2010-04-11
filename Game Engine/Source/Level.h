// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

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

// the level class (also referred to as a map or world) represents an area in which the player can explore and interact with
class Level {
public:
	Level(const char * fileName, SpriteSheets * externalSpriteSheets, Variables * settings, double & timeElapsed, int windowWidth, int windowHeight, LPDIRECT3DDEVICE9 d3dDevice);
	~Level();

	bool checkCollision(const D3DXVECTOR2 & lastPosition, const D3DXVECTOR2 & newPosition, D3DXVECTOR2 * intersection, double * newY); // check if an object is colliding with one of the boundary lines

	void tick(); // update the level
	void draw(LPDIRECT3DDEVICE9 d3dDevice); // render the level

private:
	void readFrom(ifstream & in); // read the level from an input stream

public:
	// level data
	Player * player;
	Pet * pet;
	vector<Object *> objects;
	vector<Object *> tiles;
	vector<AI *> ai;
	vector<Object *> items;

	// level size values
	int xDimension;
	int yDimension;
	int mapWidth;
	int mapHeight;

private:
	// level data
	char * name;
	Graph boundaries;
	Vertex playerSpawn;
	Vertex petSpawn;

	// external data
	SpriteSheets * spriteSheets;
	double & timeElapsed;
	int windowWidth;
	int windowHeight;
	int scrollingOffset;
	LPDIRECT3DDEVICE9 d3dDevice;
};
