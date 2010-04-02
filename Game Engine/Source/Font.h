#pragma once

#include "Includes.h"

class Font {
public:
	Font(const char * fontFace, int fontSize, int fontWeight, bool fontItalic, LPDIRECT3DDEVICE9 ext_d3dDevice);
	Font(const Font & x);
	Font & operator = (const Font & x);	
	~Font();

	int getSize() const;
	LPD3DXFONT getD3DFont() const;

private:
	void createFont();

public:
	const static int NORMAL;
	const static int REGULAR;
	const static int BOLD;

private:
	LPD3DXFONT d3dFont;
	LPDIRECT3DDEVICE9 d3dDevice;
	char * face;
	int size;
	int weight;
	bool italic;
};
