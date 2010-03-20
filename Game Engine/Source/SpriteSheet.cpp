#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(Sprite * externalSpriteSheet,
						 SpriteSheetOffset * offsets,
						 int numberOfSprites)
							: spriteSheet(externalSpriteSheet) {
	for(int i=0;i<numberOfSprites;i++) {
		sprites.push_back(new Sprite(offsets[i].x,
									 offsets[i].y,
									 offsets[i].w,
									 offsets[i].h,
									 spriteSheet->getTexture(),
									 spriteSheet->getSprite()));
	}
}

SpriteSheet::SpriteSheet(Sprite * externalSpriteSheet,
						 int xOffset,
						 int yOffset,
						 int width,
						 int height,
						 int xIncrement,
						 int yIncrement,
						 int numberOfSprites)
							: spriteSheet(externalSpriteSheet) {
	for(int i=0;i<numberOfSprites;i++) {
		sprites.push_back(new Sprite(xOffset + (xIncrement * i),
									 yOffset + (yIncrement * i),
									 width,
									 height,
									 spriteSheet->getTexture(),
									 spriteSheet->getSprite()));
	}
}

SpriteSheet::SpriteSheet(Sprite * externalSpriteSheet,
						 int xOffset,
						 int yOffset,
						 int width,
						 int height,
						 int xIncrement,
						 int yIncrement,
						 bool horizontal,
						 int numberOfRows,
						 int numberOfColumns)
							: spriteSheet(externalSpriteSheet) {
	int xPos = xOffset;
	int yPos = yOffset;
	for(int i=0;i<numberOfRows;i++) {
		for(int j=0;j<numberOfColumns;j++) {
			sprites.push_back(new Sprite(xPos,
										 yPos,
										 width,
										 height,
										 spriteSheet->getTexture(),
										 spriteSheet->getSprite()));
			if(horizontal) { xPos += xIncrement; }
			else { yPos += yIncrement; }
		}
		if(horizontal) { yPos += yIncrement; xPos = xOffset; }
		else { xPos += xIncrement; yPos = yOffset; }
	}
}

SpriteSheet::~SpriteSheet() {
	for(unsigned int i=0;i<sprites.size();i++) {
		if(sprites.at(i) != NULL) { delete sprites.at(i); }
	}
}

Sprite * SpriteSheet::getSprite(int index) {
	if(index < 0 || index >= (int) sprites.size()) { return NULL; }
	return sprites.at(index);
}

vector<Sprite *> * SpriteSheet::getSprites(int startIndex, int endIndex) {
	if(startIndex < 0 || endIndex >= (int) sprites.size()) { return NULL; }
	vector<Sprite *> * spriteGroup = new vector<Sprite *>;
	for(int i=startIndex;i<=endIndex;i++) {
		spriteGroup->push_back(sprites.at(i));
	}
	return spriteGroup;
}
