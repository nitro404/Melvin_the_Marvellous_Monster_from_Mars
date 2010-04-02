#pragma once

#include "Includes.h"
#include "Variables.h"
#include "Text.h"

class Game;

// simple menu abstraction (only used for storing, rendering and interaction - function calls done by game engine)
class Menu {
public:
	Menu(int windowWidth, int windowHeight, Game * game, Variables * settings, LPDIRECT3DDEVICE9 d3dDevice);
	~Menu();

	void select();
	void back();
	void moveUp(); // move menu selection up
	void moveDown(); // move menu selection down

	bool isActive();
	void activate();
	void deactivate();
	void toggle();

	void draw(LPDIRECT3DDEVICE9 d3dDevice); // draw the menu

private:
	void loadMapList(const char * mapDirectory);
	void setMenu(int type);
	void pauseGame();
	void resumeGame();

public:
	const static int MENU_MAIN;
	const static int MENU_NEWGAME;
	const static int MENU_HELP;

private:
	bool active;
	int menuType;

	Game * game;

	int mainMenuIndex;
	Text * mainMenuTitle;
	Text * mainMenuTitle2;
	vector<Text *> mainMenuItems;

	int mapMenuIndex;
	Text * mapMenuTitle;
	vector<Text *> mapMenuItems;
	vector<string> mapList;

	Text * helpMenuTitle;
	vector<Text *> helpMenuItems;

	Font * titleFont;
	Font * itemFont;

	// menu colours
	D3DCOLOR titleColour;
	D3DCOLOR activeColour;
	D3DCOLOR inactiveColour;

	// menu draw position information
	int titleOffset;
	int menuItemOffset;
	int menuItemIncrement;

	// window dimensions
	int windowWidth;
	int windowHeight;
};
