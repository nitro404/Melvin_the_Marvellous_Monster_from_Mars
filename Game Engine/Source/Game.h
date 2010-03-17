#pragma once

#include "Includes.h"
#include "DirectX.h"
#include "Variables.h"
#include "Sprite.h"
#include "Menu.h"
#include "Object.h"
#include "Player.h"

class Game {
public:
	Game(Variables * settings, HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow);
	~Game();
	
	int run();
	void tick();
	void drawHelp();
	void draw();
	void reset();
	
//TODO:
	bool loadMap(char * fileName);
	
private:
	void processKeyboardInput();
	void processMouseInput();
	void processMainMenuInput();
	void processPlayerInput();

	bool init(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow);
	bool RegisterWndClass(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName);
	bool verifySettings(Variables * settings);

private:
	int windowPosX, windowPosY;
	int windowWidth, windowHeight;

	Variables * settings;

//private:
//	Sprite * playerSprite;
	Player * player;

	Menu * mainMenu;
	bool mainMenuActive;
	Text * helpTitleText;
	vector<char *> helpItemMessages;
	vector<Text *> helpItemText;
	int helpItemOffset;
	int helpItemIncrement;
	bool helpScreenActive;

//	bool leftMouseButtonPressed;
//	bool rightMouseButtonPressed;
	bool spaceBarPressed;
	bool enterKeyPressed;
	bool menuUpKeyPressed;
	bool menuDownKeyPressed;
	bool menuSelectKeyPressed;
	bool escapeKeyPressed;

	HINSTANCE hInstance;
	HWND hWnd;

	LPDIRECTINPUT8 directInput;
	LPDIRECTINPUTDEVICE8 keyboard;
	LPDIRECTINPUTDEVICE8 mouse;
	char keyboardState[256];
	DIMOUSESTATE mouseState;
	
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3dDevice;
	
	time_t lastRenderTime;
	int fps;
	time_t timePerFrame;
};
