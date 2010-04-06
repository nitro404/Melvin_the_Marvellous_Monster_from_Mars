#pragma once

#include "Includes.h"
#include "Variables.h"
#include "Sprite.h"

struct SpriteSheetOffset {
	int x, y;
	int w, h;
};

class SpriteSheet {
public:
	SpriteSheet(Sprite * externalSpriteSheet,
				SpriteSheetOffset * offsets,
				int numberOfSprites);

	SpriteSheet(Sprite * externalSpriteSheet,
				int xOffset,
				int yOffset,
				int width,
				int height,
				int xIncrement,
				int yIncrement,
				bool horizontal,
				int numberOfRows,
				int numOfColumns);
	
	~SpriteSheet();

	int size();
	Sprite * elementAt(int index);
	Sprite * getSprite(int index);
	Sprite * getSprite(char * name);
	vector<Sprite *> * getSprites(int startIndex, int endIndex);

	char * getName();
	void setName(char * name);

	static int SpriteSheet::parseType(const char * data);
	static SpriteSheet * parseFrom(ifstream & in, char * spriteDirectory, LPDIRECT3DDEVICE9 d3dDevice);

public:
	const static int TYPE_INVALID;
	const static int TYPE_ARBITRARY_OFFSETS;
	const static int TYPE_SINGLE_GRID;
	const static int TYPE_MULTIPLE_GRIDS;

private:
	char * name;
	vector<Sprite *> sprites;
	Sprite * spriteSheet;
};
