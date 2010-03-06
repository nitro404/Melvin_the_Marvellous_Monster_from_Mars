#pragma once

#include "Includes.h"
#include "DirectX.h"
#include "Variables.h"
#include "Sprite.h"
#include "Object.h"
#include "Player.h"

class Game {
public:
	Game(Variables * settings, HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow);
	~Game();
	
	void processKeyboardInput();
	void processMouseInput();
	
	int run();
	void tick();
	void draw();
	
//TODO:
	bool loadMap(char * fileName);
	
private:
	bool init(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow);
	bool RegisterWndClass(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName);
	bool verifySettings(Variables * settings);

public:
	int windowPosX, windowPosY;
	int windowWidth, windowHeight;
	Variables * settings;

private:
	Sprite * playerSprite;
	Player * player;

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
