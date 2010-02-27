#ifndef _GAME_H
#define _GAME_H

#include "Includes.h"

class Game {
public:
	Game(int posX, int posY, int width, int height, HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow);
	~Game();
	
	void processKeyboardInput();
	void processMouseInput();
	
	int run();
	void tick();
	void draw();

private:
	int init(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow);
	int RegisterWndClass(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName);

public:
	int windowPosX, windowPosY;
	int windowWidth, windowHeight;

private:
	HINSTANCE hInstance;
	HWND hWnd;

	LPDIRECTINPUT8 directInput;
	LPDIRECTINPUTDEVICE8 keyboard;
	LPDIRECTINPUTDEVICE8 mouse;
	char keyboardState[256];
	DIMOUSESTATE mouseState;
	
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3dDev;
	
	time_t lastRenderTime;
	int fps;
	time_t timePerFrame;
};

#endif
