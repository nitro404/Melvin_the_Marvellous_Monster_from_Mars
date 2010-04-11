// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#include "Includes.h"
#include "Variables.h"
#include "Text.h"

class Game;

// the menu class represents a menu which provides functionality for interacting with a game object
class Menu {
public:
	Menu(int windowWidth, int windowHeight, Game * game, Variables * settings, LPDIRECT3DDEVICE9 d3dDevice);
	~Menu();

	void select(); // activate the currently selected menu item
	void back(); // move the menu back
	void moveUp(); // move menu selection up
	void moveDown(); // move menu selection down

	bool isActive(); // return true if the menu is currently active
	void activate(); // activate the menu
	void deactivate(); // deactivate the menu
	void toggle(); // toggle the menu

	void draw(LPDIRECT3DDEVICE9 d3dDevice); // render the menu

private:
	void loadMapList(const char * mapDirectory); // generate a list of map paths from the specified directory
	void setMenu(int type); // change to a different menu (or sub-menu)
	void pauseGame(); // pause the game (show the menu)
	void resumeGame(); // resume the game (hide the menu)

public:
	// menu type constants
	const static int MENU_MAIN;
	const static int MENU_NEWGAME;
	const static int MENU_HELP;

private:
	// menu location and state
	bool active;
	int menuType;

	// a reference to the game the menu controls
	Game * game;

	// main menu variables
	int mainMenuIndex;
	Text * mainMenuTitle;
	Text * mainMenuTitle2;
	vector<Text *> mainMenuItems;

	// map menu variables
	int mapMenuIndex;
	Text * mapMenuTitle;
	vector<Text *> mapMenuItems;
	vector<string> mapList;

	// help menu variables
	Text * helpMenuTitle;
	vector<Text *> helpMenuItems;

	// menu fonts
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
