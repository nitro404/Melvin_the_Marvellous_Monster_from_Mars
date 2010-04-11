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
#include "Sprite.h"

// a struct representing a bounding box for a sprite within a sprite sheet
struct SpriteSheetOffset {
	int x, y;
	int w, h;
};

// the sprite sheet class represents a collection of sprites (images) which are part of a larger sprite sheet (image)
// has support for parsing sprite sheets from a specific set of attributes
// as well as reading a list of sprite sheet data from a file, then parsing and creating a corresponding sprite sheet
//
// there are 3 types of sprite sheets:
// 1) Arbitrary Offsets - A collection of offsets (x, y) and sizes (width, height) which each represent a sub-image (sprite) within a larger image (sprite sheet).
//                        Can be instantiated using a constructor or parsed from a file using an array of spritesheetoffset structs. Name and type can also be specified
//                        for each individual sprite if using the file parser in the order they are to be parsed.
//
// 2) Single Grid - Only requires a single offset (x, y) and a single size (width, height) as well as a specification indicating the number of rows and columns, and
//                  the direction to parse (vertical or horizontal). Name and type can also be specified for each sprite in the order they are to be parsed.
//                  The constructor will automatically loop through, incrementing as appropriate and creating each sprite from the sprite sheet.
//
// 3) Multiple Grids - This specification currently only works from the data file parser and does not have a constructor available.
//                     Requires the same specifications as a single grid for each grid it is to parse within a single image - these specifications include
//                     offset of the current grid (x, y), sizes of each item in the current grid (width, height), number of rows and columns in the current grid
//                     and the direction to parse the current grid (vertical or horizontal).
class SpriteSheet {
public:
	// construct an empty sprite sheet
	SpriteSheet::SpriteSheet();

	// construct an arbitrary offset sprite sheet from a collection of offsets
	SpriteSheet(Sprite * externalSpriteSheet,
				SpriteSheetOffset * offsets,
				int numberOfSprites);

	// construct a single grid sprite sheet from the corresponding specifications
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
	
	// destructor
	~SpriteSheet();

	int size() const; // return the number of sprite within the sprite sheet
	Sprite * getImage() const; // return the original sprite sheet image as a sprite
	void setImage(Sprite * spriteSheet); // set the sprite sheet image to a new image
	void addSprite(Sprite * sprite); // add a sprite to the sprite collection
	Sprite * elementAt(int index); // get a sprite from the sprite collection at a specific index
	Sprite * getSprite(int index); // get a sprite from the sprite collection at a specific index
	Sprite * getSprite(char * name); // get a sprite from the sprite collection by name
	vector<Sprite *> * getSprites(int startIndex, int endIndex); // get a sub-collection of sprites from the sprite collection

	char * getName(); // get the name of the sprite sheet
	void setName(char * name); // set the name of the sprite sheet

	static int parseType(const char * data); // parse the sprite sheet type from a string
	static SpriteSheet * parseFrom(ifstream & in, const char * spriteDirectory, LPDIRECT3DDEVICE9 d3dDevice); // parse the sprite sheet from an input stream

public:
	// sprite sheet type constants
	const static int TYPE_INVALID;
	const static int TYPE_ARBITRARY_OFFSETS;
	const static int TYPE_SINGLE_GRID;
	const static int TYPE_MULTIPLE_GRIDS;

private:
	// sprite sheet data
	char * name;
	vector<Sprite *> sprites;
	Sprite * spriteSheet;
};
