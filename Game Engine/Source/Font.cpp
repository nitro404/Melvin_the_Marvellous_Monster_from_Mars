#include "Font.h"

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
	if(fontFace == NULL) {
		this->face = new char[6];
		strcpy_s(this->face, 6, "Arial");
	}
	else {
		face = new char[strlen(fontFace) + 1];
		strcpy_s(this->face, strlen(fontFace) + 1, fontFace);
	}

	createFont();
}

Font::Font(const Font & x) {
	this->face = new char[strlen(x.face) + 1];
	strcpy_s(this->face, strlen(x.face) + 1, x.face);

	this->size = x.size;
	this->weight = x.weight;
	this->italic = x.italic;
	this->d3dDevice = x.d3dDevice;

	createFont();
}

Font & Font::operator = (const Font & x) {
	delete [] this->face;
	this->d3dFont->Release();

	this->face = new char[strlen(x.face) + 1];
	strcpy_s(this->face, strlen(x.face) + 1, x.face);

	this->size = x.size;
	this->weight = x.weight;
	this->italic = x.italic;
	this->d3dDevice = x.d3dDevice;

	createFont();

	return *this;
}

Font::~Font() {
	delete [] face;
	d3dFont->Release();
}

void Font::createFont() {
	D3DXCreateFont(d3dDevice, size, 0, weight, 0, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, face, &d3dFont);
}

int Font::getSize() const { return size; }

LPD3DXFONT Font::getD3DFont() const { return this->d3dFont; }