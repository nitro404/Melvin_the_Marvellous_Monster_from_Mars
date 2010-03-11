#pragma once

#include "Includes.h"
#include "DirectX.h"

class Text {
public:
	Text(const char * fontFace, int fontSize, int fontWeight, bool italic, int fontAlignment, int fontOffset, int xPos, int yPos, D3DCOLOR fontColour, LPDIRECT3DDEVICE9 d3dDevice);
	~Text();

	void draw(const char * text, ...);

	void setColour(D3DCOLOR newColor);
	void setPosition(int x, int y);

	const static int NORMAL;
	const static int REGULAR;
	const static int BOLD;

	const static int LEFT;
	const static int CENTER;
	const static int RIGHT;

private:
	void drawHelper(const char * text);

private:
	char * text;

	LPD3DXFONT font;
	LPD3DXSPRITE sprite;

	int size;
	int x, y;
	int alignment;
	int offset;

	D3DCOLOR colour;

	RECT textBox;
};
