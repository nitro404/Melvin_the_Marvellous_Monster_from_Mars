// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#include "Includes.h"
#include "DirectX.h"
#include "Variables.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "SpriteSheets.h"
#include "Menu.h"
#include "Object.h"
#include "Player.h"
#include "Level.h"

// the game class represents a full implementation of a 2D sidescrolling world that the user can interact with
class Game {
public:
	Game(Variables * settings, HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, int nCmdShow);
	~Game();
	
	int run(); // run the game
	void tick(); // update the game
	void draw(); // render the game
	void reset(); // reset the game to its original state within the current level
	
	void closeLevel(); // close the currently open level
	void loadLevel(const char * fileName); // load a new level
	
private:
	void computeTimeElapsed(); // update the time elapsed since the last call to this function

	void processKeyboardInput(); // process keyboard input
	void processMouseInput(); // process mouse input
	void processMenuInput(); // process input related to the menu
	void processPlayerInput(); // process input related to the player

	bool init(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow); // initialise the game
	bool RegisterWndClass(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName); // register the window
	bool verifySettings(Variables * settings); // verify the settings file contents

public:
	double timeElapsed;

private:
	// game data
	Level * level;
	D3DCOLOR backgroundColour;
	SpriteSheets * spriteSheets;

	// the game settings
	Variables * settings;

	// window information
	int windowPosX, windowPosY;
	int windowWidth, windowHeight;

	// the menu object
	Menu * menu;

	// input semaphores
	bool menuSelectKeyPressed;
	bool menuBackKeyPressed;
	bool menuUpKeyPressed;
	bool menuDownKeyPressed;

	// window handles
	HINSTANCE hInstance;
	HWND hWnd;

	// direct input handles
	LPDIRECTINPUT8 directInput;
	LPDIRECTINPUTDEVICE8 keyboard;
	LPDIRECTINPUTDEVICE8 mouse;
	char keyboardState[256];
	DIMOUSESTATE mouseState;
	
	// directx handles
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3dDevice;
	
	// fps data
	time_t lastRenderTime;
	int fps;
	time_t timePerFrame;
};
