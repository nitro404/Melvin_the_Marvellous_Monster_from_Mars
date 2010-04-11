// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#include "Includes.h"
#include "Sprite.h"
#include "SpriteSheet.h"

class SpriteSheets {
public:
	SpriteSheets();
	~SpriteSheets();

	int size(); // return the number of sprite sheets in the sprite sheet collection
	SpriteSheet * elementAt(int index); // get a sprite sheet at a specific index
	SpriteSheet * getSpriteSheet(int index); // get a sprite sheet at a specific index
	SpriteSheet * getSpriteSheet(char * name); // get a sprite sheet by name
	int getSpriteSheetIndex(char * name); // get the index of a sprite sheet by its name
	Sprite * getSprite(char * name); // get a sprite from any sprite sheet by its name
	bool add(SpriteSheet * spriteSheet); // add a sprite sheet to the sprite sheet collection
	static SpriteSheets * parseFrom(char * fileName, char * spriteDirectory, LPDIRECT3DDEVICE9 d3dDevice); // parse a collection of sprite sheets from a file

private:
	// sprite sheet collection
	vector<SpriteSheet *> spriteSheets;
};
