#include "Text.h"

const int Text::NORMAL = FW_NORMAL;
const int Text::REGULAR = FW_REGULAR;
const int Text::BOLD = FW_BOLD;

const int Text::LEFT = DT_LEFT;
const int Text::CENTER = DT_CENTER;
const int Text::RIGHT = DT_RIGHT;

Text::Text(const char * fontFace,
		   int fontSize,
		   int fontWeight,
		   bool italic,
		   int fontAlignment,
		   int fontOffset,
		   int xPos,
		   int yPos,
		   D3DCOLOR fontColour,
		   LPDIRECT3DDEVICE9 d3dDevice)
			: size(fontSize),
			  x(xPos),
			  y(yPos),
			  alignment(fontAlignment),
			  offset(fontOffset),
			  colour(fontColour) {
	D3DXCreateSprite(d3dDevice, &sprite);

	USES_CONVERSION;
	D3DXCreateFont(d3dDevice, size, 0, fontWeight, 0, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, A2W(fontFace), &font);
}

Text::~Text() {
	if(font != NULL) { font->Release(); }
	if(sprite != NULL) { sprite->Release(); }
}

void Text::draw(const char * text, ...) {
	int length = strlen(text);

	char buffer[256];
	va_list parameters;
	va_start(parameters, text);
	vsprintf_s(buffer, 256, text, parameters);
	va_end(parameters);

	drawHelper(buffer);
}

void Text::drawHelper(const char * text) {
	int length = strlen(text);
	int width = (int) ((size * length) / 2.0f);

	textBox.top = y;
	textBox.bottom = y + size;
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

	USES_CONVERSION;
	font->DrawText(sprite, A2W(text), length, &textBox, alignment, colour);

	sprite->End();
}
