// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#include "Includes.h"

// the font object represents a wrapper around a directx font which allows for simpler fonts
class Font {
public:
	Font(const char * fontFace, int fontSize, int fontWeight, bool fontItalic, LPDIRECT3DDEVICE9 ext_d3dDevice);
	Font(const Font & x);
	Font & operator = (const Font & x);	
	~Font();

	int getSize() const; // returns the size of the font
	LPD3DXFONT getD3DFont() const; // returns the handle to the directx font

private:
	void createFont(); // creates the font based on the local variable specifications

public:
	// font weight constants
	const static int NORMAL;
	const static int REGULAR;
	const static int BOLD;

private:
	// directx font handles
	LPD3DXFONT d3dFont;
	LPDIRECT3DDEVICE9 d3dDevice;

	// font specification variables
	char * face;
	int size;
	int weight;
	bool italic;
};
