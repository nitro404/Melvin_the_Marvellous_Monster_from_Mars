#include "Includes.h"
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

	Sprite * getSprite(int index);
	vector<Sprite *> * getSprites(int startIndex, int endIndex);

private:
	vector<Sprite *> sprites;
	Sprite * spriteSheet;
};
