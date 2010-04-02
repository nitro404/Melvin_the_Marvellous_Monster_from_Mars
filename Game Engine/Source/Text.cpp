#include "Text.h"

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
	D3DXCreateSprite(d3dDevice, &sprite);
	
	if(text == NULL) {
		this->text = new char[1];
		this->text[0] = '\0';
	}
	else {
		char buffer[256];
		va_list parameters;
		va_start(parameters, text);
		vsprintf_s(buffer, 256, text, parameters);
		va_end(parameters);

		this->text = new char[strlen(buffer) + 1];
		strcpy_s(this->text, strlen(buffer) + 1, buffer);
	}

	if(font == NULL) {
		this->font = new Font("Arial", 24, Font::REGULAR, false, d3dDevice);
		externalFont = false;
	}
	if(this->font == NULL) {
		this->font = new Font(*font);
		externalFont = false;
	}
}

Text::~Text() {
	delete [] text;
	if(!externalFont) { delete font; }
	if(sprite != NULL) { sprite->Release(); }
}

int Text::getSize() { return font->getSize(); }

void Text::setColour(D3DCOLOR newColour) { colour = newColour; }

void Text::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
}

char * Text::getText() { return text; }

void Text::setText(const char * text, ...) {
	delete [] text;

	char buffer[256];
	va_list parameters;
	va_start(parameters, text);
	vsprintf_s(buffer, 256, text, parameters);
	va_end(parameters);

	this->text = new char[strlen(buffer) + 1];
	strcpy_s(this->text, strlen(buffer) + 1, buffer);
}

void Text::draw() {
	draw(this->text);
}

void Text::draw(const char * text, ...) {
	char buffer[256];
	va_list parameters;
	va_start(parameters, text);
	vsprintf_s(buffer, 256, text, parameters);
	va_end(parameters);

	int length = strlen(buffer);
	int width = (int) ((font->getSize() * length) / 2.0f);

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

	font->getD3DFont()->DrawText(sprite, buffer, length, &textBox, alignment, colour);

	sprite->End();
}
