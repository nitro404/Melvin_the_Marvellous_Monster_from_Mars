#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(Sprite * externalSpriteSheet,
						 int x,
						 int y,
						 int w,
						 int h,
						 int xInc,
						 int yInc,
						 int count)
							: spriteSheet(externalSpriteSheet),
							  xOffset(x),
							  yOffset(y),
							  width(w),
							  height(h),
							  xIncrement(xInc),
							  yIncrement(y),
							  numberOfSprites(count) {
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
						 SpriteSheetOffset * offsets,
						 int count)
							: spriteSheet(externalSpriteSheet),
							  xOffset(-1),
							  yOffset(-1),
							  xIncrement(-1),
							  yIncrement(-1),
							  numberOfSprites(count) {
	for(int i=0;i<numberOfSprites;i++) {
		sprites.push_back(new Sprite(offsets[i].x,
									 offsets[i].y,
									 offsets[i].w,
									 offsets[i].h,
									 spriteSheet->getTexture(),
									 spriteSheet->getSprite()));
	}
}

SpriteSheet::~SpriteSheet() {
	for(unsigned int i=0;i<sprites.size();i++) {
		if(sprites.at(i) != NULL) { delete sprites.at(i); }
	}
}

Sprite * SpriteSheet::getSprite(int index) {
	return sprites.at(index);
}
