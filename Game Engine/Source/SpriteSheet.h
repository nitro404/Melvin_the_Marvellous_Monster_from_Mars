#include "Includes.h"
#include "Sprite.h"

struct SpriteSheetOffset {
	int x, y;
	int w, h;
};

class SpriteSheet {
public:
	SpriteSheet::SpriteSheet(Sprite * externalSpriteSheet, int x, int y, int w, int h, int xInc, int yInc, int count);
	SpriteSheet::SpriteSheet(Sprite * externalSpriteSheet, SpriteSheetOffset * offsets, int count);
	~SpriteSheet();

	Sprite * SpriteSheet::getSprite(int index);

private:
	vector<Sprite *> sprites;

	int xOffset, yOffset;
	int xIncrement, yIncrement;
	int width, height;
	int numberOfSprites;

	Sprite * spriteSheet;
};
