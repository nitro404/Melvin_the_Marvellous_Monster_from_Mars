// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#include "Includes.h"
#include "Font.h"

// the text class represents a visual version of a string which can be rendered to the screen
class Text {
public:
	Text(int xPos, int yPos, int fontAlignment, int fontOffset, D3DCOLOR fontColour, Font * font, bool isExternalFont, LPDIRECT3DDEVICE9 d3dDevice, char * text, ...);
	~Text();

	int getSize(); // returns the size of the text
	void setColour(D3DCOLOR newColor); // changes the colour of the text
	void setPosition(int x, int y); // changes the location of the text on the screen
	char * getText(); // returns the string currently stored in the text
	void setText(const char * text, ...); // changes the string currently stored in the text

	void draw(); // render the text
	void draw(const char * text, ...); // render the text with an alternate string

public:
	// aligntment and offset constants
	const static int LEFT;
	const static int CENTER;
	const static int RIGHT;

private:
	// text data
	char * text;
	Font * font;
	LPD3DXSPRITE sprite;
	bool externalFont;

	// text location variables
	int x, y;
	int alignment;
	int offset;

	// text colour
	D3DCOLOR colour;

	// text bounding box
	RECT textBox;
};
