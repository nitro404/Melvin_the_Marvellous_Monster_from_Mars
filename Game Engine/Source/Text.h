#pragma once

#include "Includes.h"
#include "Font.h"

class Text {
public:
	Text(int xPos, int yPos, int fontAlignment, int fontOffset, D3DCOLOR fontColour, Font * font, bool isExternalFont, LPDIRECT3DDEVICE9 d3dDevice, char * text, ...);
	~Text();

	int getSize();
	void setColour(D3DCOLOR newColor);
	void setPosition(int x, int y);
	char * getText();
	void setText(const char * text, ...);

	void draw();
	void draw(const char * text, ...);

public:
	const static int LEFT;
	const static int CENTER;
	const static int RIGHT;

private:
	char * text;
	Font * font;
	LPD3DXSPRITE sprite;
	bool externalFont;

	int x, y;
	int alignment;
	int offset;

	D3DCOLOR colour;

	RECT textBox;
};
