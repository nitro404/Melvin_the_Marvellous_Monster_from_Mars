#pragma once

#include "Includes.h"
#include "SpriteSheet.h"
#include "Items.h"

class Level : public Items {
public:
	Level();
	Level(char * fileName);
	~Level();

	void tick();
	void draw(LPDIRECT3DDEVICE9 d3dDevice);

	void readFrom(ifstream & in);

private:
	vector<SpriteSheet *> levelSpriteSheets;
	vector<Sprite *> levelSpriteSheetImages;
};
