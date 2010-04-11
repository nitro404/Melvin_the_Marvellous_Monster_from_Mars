// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#include "Font.h"

// font weight constants
const int Font::NORMAL = FW_NORMAL;
const int Font::REGULAR = FW_NORMAL;
const int Font::BOLD = FW_NORMAL;

Font::Font(const char * fontFace,
		   int fontSize,
		   int fontWeight,
		   bool fontItalic,
		   LPDIRECT3DDEVICE9 ext_d3dDevice)
		   : face(NULL),
			 size((fontSize < 0) ? 24 : fontSize),
			 weight((fontWeight == NORMAL || fontWeight == REGULAR || fontWeight == BOLD) ? fontWeight : NORMAL),
			 italic(fontItalic),
			 d3dDevice(ext_d3dDevice) {
	// if no face name is specified, default to Arial
	if(fontFace == NULL) {
		this->face = new char[6];
		strcpy_s(this->face, 6, "Arial");
	}
	// otherwise copy the face name
	else {
		face = new char[strlen(fontFace) + 1];
		strcpy_s(this->face, strlen(fontFace) + 1, fontFace);
	}

	// create the font
	createFont();
}

Font::Font(const Font & x) {
	// copy the face name
	this->face = new char[strlen(x.face) + 1];
	strcpy_s(this->face, strlen(x.face) + 1, x.face);

	// copy the other local variables
	this->size = x.size;
	this->weight = x.weight;
	this->italic = x.italic;
	this->d3dDevice = x.d3dDevice;

	// create the font
	createFont();
}

Font & Font::operator = (const Font & x) {
	// delete the old font
	delete [] this->face;
	this->d3dFont->Release();

	// copy the face name
	this->face = new char[strlen(x.face) + 1];
	strcpy_s(this->face, strlen(x.face) + 1, x.face);

	// copy the other local variables
	this->size = x.size;
	this->weight = x.weight;
	this->italic = x.italic;
	this->d3dDevice = x.d3dDevice;

	// create the font
	createFont();

	return *this;
}

Font::~Font() {
	delete [] face;
	d3dFont->Release();
}

// creates the font based on the local variable specifications
void Font::createFont() {
	D3DXCreateFont(d3dDevice, size, 0, weight, 0, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, face, &d3dFont);
}

// returns the size of the font
int Font::getSize() const { return size; }

// returns the handle to the directx font
LPD3DXFONT Font::getD3DFont() const { return this->d3dFont; }
