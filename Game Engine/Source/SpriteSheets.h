#pragma once

#include "Includes.h"
#include "Sprite.h"
#include "SpriteSheet.h"

class SpriteSheets {
public:
	SpriteSheets();
	~SpriteSheets();

	int size();
	SpriteSheet * elementAt(int index);
	SpriteSheet * getSpriteSheet(int index);
	SpriteSheet * getSpriteSheet(char * name);
	int getSpriteSheetIndex(char * name);
	Sprite * getSprite(char * name);
	bool add(SpriteSheet * spriteSheet);
	static SpriteSheets * parseFrom(char * fileName, char * spriteDirectory, LPDIRECT3DDEVICE9 d3dDevice);

private:
	vector<SpriteSheet *> spriteSheets;
};
