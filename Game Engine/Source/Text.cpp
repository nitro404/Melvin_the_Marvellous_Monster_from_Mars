// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#include "Text.h"

// aligntment and offset constants
const int Text::LEFT = DT_LEFT;
const int Text::CENTER = DT_CENTER;
const int Text::RIGHT = DT_RIGHT;

Text::Text(int xPos,
		   int yPos,
		   int fontAlignment,
		   int fontOffset,
		   D3DCOLOR fontColour,
		   Font * font,
		   bool isExternalFont,
		   LPDIRECT3DDEVICE9 d3dDevice,
		   char * text, ...)
			: x(xPos),
			  y(yPos),
			  font((isExternalFont) ? font : NULL),
			  externalFont(isExternalFont),
			  alignment(fontAlignment),
			  offset(fontOffset),
			  colour(fontColour) {
	// create a directx sprite to render the text info
	D3DXCreateSprite(d3dDevice, &sprite);
	
	// instantiate the string stored inside the text
	if(text == NULL) {
		this->text = new char[1];
		this->text[0] = '\0';
	}
	else {
		// print the parameter list to the string
		char buffer[256];
		va_list parameters;
		va_start(parameters, text);
		vsprintf_s(buffer, 256, text, parameters);
		va_end(parameters);

		// copy the buffer to the string
		this->text = new char[strlen(buffer) + 1];
		strcpy_s(this->text, strlen(buffer) + 1, buffer);
	}

	// if no font was specified, set the font to the default font
	if(font == NULL) {
		this->font = new Font("Arial", 24, Font::REGULAR, false, d3dDevice);
		externalFont = false;
	}

	// if the font is an internal font, create a copy of the font
	if(this->font == NULL) {
		this->font = new Font(*font);
		externalFont = false;
	}
}

Text::~Text() {
	delete [] text;
	if(!externalFont) { delete font; } // only delete the font if it is an internal font
	if(sprite != NULL) { sprite->Release(); }
}

// returns the size of the text
int Text::getSize() { return font->getSize(); }

// changes the colour of the text
void Text::setColour(D3DCOLOR newColour) { colour = newColour; }

// changes the location of the text on the screen
void Text::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
}

// returns the string currently stored in the text
char * Text::getText() { return text; }

// changes the string currently stored in the text
void Text::setText(const char * text, ...) {
	delete [] text;

	// print the parameter list to the string
	char buffer[256];
	va_list parameters;
	va_start(parameters, text);
	vsprintf_s(buffer, 256, text, parameters);
	va_end(parameters);

	// copy the buffer to the string data stored in the text
	this->text = new char[strlen(buffer) + 1];
	strcpy_s(this->text, strlen(buffer) + 1, buffer);
}

// render the text
void Text::draw() {
	draw(this->text);
}

// render the text with an alternate string
void Text::draw(const char * text, ...) {
	// print the parameter list to the string
	char buffer[256];
	va_list parameters;
	va_start(parameters, text);
	vsprintf_s(buffer, 256, text, parameters);
	va_end(parameters);

	// compute the length of the string and the width of the bounding box
	int length = strlen(buffer);
	int width = (int) ((font->getSize() * length) / 2.0f);

	// create the text bounding box
	textBox.top = y;
	textBox.bottom = y + font->getSize();
	if(offset == LEFT) {
		textBox.left = x - width;
		textBox.right = x;
	}
	else if(offset == RIGHT) {
		textBox.left = x;
		textBox.right = x + width;
	}
	else {
		textBox.left = (int) (x - (width / 2.0f));
		textBox.right = (int) (x + (width / 2.0f));
	}

	sprite->Begin(D3DXSPRITE_ALPHABLEND);

	// render the text
	font->getD3DFont()->DrawText(sprite, buffer, length, &textBox, alignment, colour);

	sprite->End();
}
