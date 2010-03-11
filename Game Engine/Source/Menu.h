#pragma once

#include "Includes.h"
#include "Text.h"

// simple menu abstraction (only used for storing, rendering and interaction - function calls done by game engine)
class Menu {
public:
	Menu(const char * title, int windowWidth, int windowHeight, D3DCOLOR menuTitleColour, D3DCOLOR menuColour, D3DCOLOR menuSelectionColour, LPDIRECT3DDEVICE9 d3dDevice);
	~Menu();

	void addMenuItem(char * menuItem, LPDIRECT3DDEVICE9 d3dDevice); // add a new item to the bottom of the menu

	void moveUp(); // move menu selection up
	void moveDown(); // move menu selection down

	int getIndex(); // get the current menu position

	void draw(LPDIRECT3DDEVICE9 d3dDevice); // draw the menu

private:
	// menu data
	char * title;
	Text * titleText;
	vector<char *> menuItems;
	vector<Text *> menuItemText;
	int index;

	// menu colours
	D3DCOLOR titleColour;
	D3DCOLOR activeColour;
	D3DCOLOR inactiveColour;

	// menu draw position information
	int menuItemOffset;
	int menuItemIncrement;

	// window dimensions
	int windowWidth;
	int windowHeight;
};
