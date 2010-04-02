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

class Game {
public:
	Game(Variables * settings, HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, int nCmdShow);
	~Game();
	
	int run();
	void tick();
	void draw();
	void reset();
	
	void closeLevel();
	void loadLevel(const char * fileName);
	
private:
	void computeTimeElapsed();

	void processKeyboardInput();
	void processMouseInput();
	void processMenuInput();
	void processPlayerInput();

	bool init(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow);
	bool RegisterWndClass(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName);
	bool verifySettings(Variables * settings);

public:
	static float gravity;
	double timeElapsed;

private:
	Player * player;
	SpriteSheets * spriteSheets;

	Level * level;

	Variables * settings;

	int windowPosX, windowPosY;
	int windowWidth, windowHeight;

	Menu * menu;

	bool menuSelectKeyPressed;
	bool menuBackKeyPressed;
	bool menuUpKeyPressed;
	bool menuDownKeyPressed;

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
