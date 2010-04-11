// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#include "Game.h"

#if _DEBUG
// debug drawing
D3DXVECTOR2 playerNewPosition;
D3DXVECTOR2 playerLastPosition;

D3DXVECTOR2 playerCollisionPointA;
D3DXVECTOR2 playerCollisionPointB;
D3DXVECTOR2 playerCollisionPosition;
#endif

Game::Game(Variables * settings,
		   HINSTANCE hInstance,
		   WNDPROC WndProc,
		   LPCTSTR winClassName,
		   int nCmdShow)
		   : menu(NULL),
			 menuSelectKeyPressed(false),
			 menuBackKeyPressed(false),
			 menuUpKeyPressed(false),
			 menuDownKeyPressed(false),
			 level(NULL),
			 backgroundColour(D3DCOLOR_XRGB(173, 244, 255)),
			 directInput(NULL),
			 keyboard(NULL),
			 mouse(NULL),
			 d3d(NULL),
			 d3dDevice(NULL),
			 timeElapsed(0) {
	// verify that the required parameters are specified in the settings file
	this->settings = settings;
	if(!verifySettings(settings)) {
		quit("Error", "Settings file is invalid.");
	}
	
	// create the window using the specifications from the settings file
	this->windowPosX = atoi(settings->getValue("Window Position Horizontal"));
	this->windowPosY = atoi(settings->getValue("Window Position Vertical"));
	this->windowWidth = atoi(settings->getValue("Window Width"));
	this->windowHeight = atoi(settings->getValue("Window Height"));
	
	// initialise the window
	if(!init(hInstance, WndProc, winClassName, "Melvin the Marvellous Monster from Mars", nCmdShow)) {
		quit("Error", "Error initializing game.");
	}

	// parse all of the SpriteSheets associated with the game
	spriteSheets = SpriteSheets::parseFrom(settings->getValue("SpriteSheet File"), settings->getValue("Sprite Directory"), d3dDevice);

	// instantiate the menu object
	menu = new Menu(windowWidth, windowHeight, this, settings, d3dDevice);

#if _DEBUG
	backgroundColour = D3DCOLOR_XRGB(0, 0, 0);
#endif
}

Game::~Game() {
	// clean up local memory
	delete settings;
	if(spriteSheets != NULL) { delete spriteSheets; }
	if(level != NULL) { delete level; }
	delete menu;

	// release input devices
	if(keyboard != NULL) {
		keyboard->Unacquire();
		keyboard->Release();
	}
	if(mouse != NULL) {
		mouse->Unacquire();
		mouse->Release();
	}
	if(directInput != NULL) {
		directInput->Release();
	}
	if(d3dDevice != NULL) {
		d3dDevice->Release();
	}
	if(d3d != NULL) {
		d3d->Release();
	}
}

// computes the time elapsed since the last call to this function
void Game::computeTimeElapsed() {
	//Returns how much time has elapsed since the first call of this function... Accurate to a microsecond...
	static INT64 countsPerSecond;
	static INT64 oldTime;
	static bool firstTime = true;
	if(firstTime) {
		firstTime = false;
		QueryPerformanceCounter((LARGE_INTEGER *) & oldTime);
		QueryPerformanceFrequency((LARGE_INTEGER *) & countsPerSecond);
	}
	
	INT64 newTime;
	QueryPerformanceCounter((LARGE_INTEGER *) & newTime);
	INT64 elapsedCounts = newTime - oldTime; 
	
	double seconds = (double) elapsedCounts / (double) countsPerSecond; //count / (counts / second) = seconds
	
	//Compute elapsed time needed for controlling frame rate independent effects.
	//If running slower than 5 frames per second, pretend it's 5 frames/sec.
	//Note: 30 frames per second means 1/30 seconds per frame = 0.03333... seconds per frame.
	static double lastTimeInSeconds = seconds; //Pretend we are running 30 frames per second on the first tick.
	double timeInSeconds = seconds;
	timeElapsed = timeInSeconds - lastTimeInSeconds;
	if(timeElapsed > 0.2) timeElapsed = 0.2; //5 frames/sec means 1 frame in 1/5 (= 0.2) seconds.
	lastTimeInSeconds = timeInSeconds;
}

// ticks (updates) the game object
void Game::tick() {
	// update the elapsed time variable
	computeTimeElapsed();

	// get input from the user
	processKeyboardInput();
	processMouseInput();
	
	// tick the level and all of the items contained within it
	if(level != NULL) { level->tick(); }
}

// renders the Game onto the screen
void Game::draw() {
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, backgroundColour, 1.0f, 0);
	
	d3dDevice->BeginScene();

	// render the Level
	if(level != NULL) { level->draw(d3dDevice); }
	
	d3dDevice->EndScene();
	
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}

// not currently implemented
void Game::reset() {
	if(level != NULL) {
		
	}
}

// the game loop
int Game::run() {
    MSG msg;
	DWORD elapsedTime = 0;
	DWORD currentTime = 0;
	
    while(true) {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if(msg.message == WM_QUIT) {
                break;
			}
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		
		// if the menu is active, only display the menu
		if(menu->isActive()) {
			processKeyboardInput();

			menu->draw(d3dDevice);
		}
		// otherwise tick and draw the game
		else {
			tick();

			draw();
		}

		// cap the frame rate at a specified frame rate (default is 60 FPS)
		elapsedTime = (DWORD) (GetTickCount() - lastRenderTime);
		if(elapsedTime < timePerFrame) {
			Sleep((DWORD) (timePerFrame - elapsedTime));
		}
		lastRenderTime = GetTickCount();
    }
	
    return msg.wParam;
}

// unloads the level if one is loaded
void Game::closeLevel() {
	if(level != NULL) {
		delete level;
		level = NULL;
	}
}

// loads a level from a specified file
void Game::loadLevel(const char * fileName) {
	closeLevel();
	level = new Level(fileName, spriteSheets, settings, timeElapsed, windowWidth, windowHeight, d3dDevice);
	reset();
}

// verifies that the settings file specifies the window width, height, sprite directory and map directory
bool Game::verifySettings(Variables * settings) {
	return	settings->hasValue("Map Directory") &&
			settings->hasValue("Sprite Directory") &&
			settings->hasValue("Window Width") &&
			settings->hasValue("Window Height");
}

// initializes the devices required to display the Game window and obtain input
bool Game::init(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow) {
	this->hInstance = hInstance;
	this->fps = 60;
	this->timePerFrame = 1000 / fps;
	
	// register and create the window
	if(!RegisterWndClass(hInstance, WndProc, winClassName)) {
		MessageBoxA(NULL, "Could not register class.", "Error", MB_OK);
		return false;
	}
	
	hWnd = CreateWindow(winClassName, title, WS_OVERLAPPEDWINDOW, windowPosX, windowPosY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
	
	if(hWnd == NULL) {
		MessageBoxA(NULL, "Could not create window.", "Error", MB_OK);
		return false;
	}
	
	// display the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// create the direct 3d device
	D3DPRESENT_PARAMETERS d3dpp;
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if(d3d == NULL) { return false; }
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = windowWidth;
	d3dpp.BackBufferHeight = windowHeight;
	d3dpp.Windowed = true;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	if(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice) != S_OK) {
		return false;
	}
	
	// create the direct input device
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **) &directInput, NULL);
	if(directInput == NULL) { return false; }
	
	// obtain a handle to the keyboard
	if(directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL) != S_OK) { return false; }
	if(keyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK) { return false; }
	if(keyboard->SetDataFormat(&c_dfDIKeyboard) != DI_OK) { return false; }
	
	// obtain a handle to the mouse
	if(directInput->CreateDevice(GUID_SysMouse, &mouse, NULL) != DI_OK) { return false; }
	if(mouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK) { return false; }
	if(mouse->SetDataFormat(&c_dfDIMouse) != DI_OK) { return false; }

	return true;
}

// registers the window
bool Game::RegisterWndClass(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName) {
	WNDCLASS wc;
	
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName  = NULL;
	wc.lpfnWndProc	 = WndProc;
	wc.lpszClassName = winClassName;
	wc.hIcon = NULL;
	
	if(RegisterClass(&wc) == 0) {
		return false;
	}
	
	return true;
}

// obtains keyboard input from the user
void Game::processKeyboardInput() {
	// if the window is not in focus, do nothing
	if(!GetFocus()) { return; }

	// obtain the state of all of the keys on the keyboard
	int result;
	while((result = keyboard->GetDeviceState(256, keyboardState)) != DI_OK) {
		switch(result) {
			case DI_OK:
				break;
			case DIERR_INPUTLOST:
			case DIERR_NOTACQUIRED:
				keyboard->Acquire();
				break;
			case E_PENDING:
				break;
			case DIERR_INVALIDPARAM:
			case DIERR_NOTINITIALIZED:
			default:
				return;
		}
	}

	// process the keys
	processMenuInput();

	processPlayerInput();
}

void Game::processMenuInput() {
	// if the window is not in focus, do nothing
	if(!GetFocus()) { return; }

	// go back in the menu (or quit the game / open the menu, depending)
	if(keyboardState[DIK_ESCAPE] & 0x80) {
		if(!menuBackKeyPressed) { menu->back(); }
		menuBackKeyPressed = true;
	}
	else { menuBackKeyPressed = false; }

	// if the menu is not active, do not accept further input to the menu
	if(!menu->isActive()) { return; }

	// select the current item in the menu
	if(keyboardState[DIK_RETURN] & 0x80 || keyboardState[DIK_NUMPADENTER] & 0x80 || keyboardState[DIK_SPACE] & 0x80) {
		if(!menuSelectKeyPressed) { menu->select(); }
		menuSelectKeyPressed = true;
	}
	else { menuSelectKeyPressed = false; }

	// move menu selection up
	if(keyboardState[DIK_UP] & 0x80 || keyboardState[DIK_W] & 0x80) {
		if(!menuUpKeyPressed) { menu->moveUp(); }
		menuUpKeyPressed = true;
	}
	else { menuUpKeyPressed = false; }

	// move menu selection down
	if(keyboardState[DIK_DOWN] & 0x80 || keyboardState[DIK_S] & 0x80) {
		if(!menuDownKeyPressed) { menu->moveDown(); }
		menuDownKeyPressed = true;
	}
	else { menuDownKeyPressed = false; }
}

void Game::processPlayerInput() {
	// if the window is not in focus, do nothing
	if(!GetFocus()) { return; }

	// if the menu is active, do not allow the user to control the player
	if(menu->isActive()) { return; }

	// if no level is loaded, do nothing
	if(level == NULL) { return; }

	// move the player left
	level->player->isMoving = false;
	if(keyboardState[DIK_LEFT] & 0x80 || keyboardState[DIK_A] & 0x80) {
		level->player->moveLeft();
		level->player->isMoving = true;
	}
	
	// move the player right
	if(keyboardState[DIK_RIGHT] & 0x80 || keyboardState[DIK_D] & 0x80) {
		level->player->moveRight();
		level->player->isMoving = true;
	}
	
	// make the player jump
	if(keyboardState[DIK_UP] & 0x80 || keyboardState[DIK_W] & 0x80 || keyboardState[DIK_SPACE] & 0x80) {
		level->player->jump();
	}

	// make the player grab an item
	if(keyboardState[DIK_G] & 0x80) {
		level->player->grab();
	}
}

void Game::processMouseInput() {
	// if the window is not in focus, do nothing
	if(!GetFocus()) { return; }

	// get the state of the mouse
	int result;
	while((result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (void *) &mouseState)) != DI_OK) {
		switch(result) {
			case DI_OK:
				break;
			case DIERR_INPUTLOST:
			case DIERR_NOTACQUIRED:
				mouse->Acquire();
				break;
			case E_PENDING:
				break;
			case DIERR_INVALIDPARAM:
			case DIERR_NOTINITIALIZED:
			default:
				return;
		}
	}

	// no current implementation for the mouse

	/*POINT pt;
	RECT wRec;

	if(mouseState.rgbButtons[0] & 0x80) {
		GetCursorPos(&pt); 
		ScreenToClient(hWnd, &pt);
		GetWindowRect(hWnd, &wRec); 

		if(pt.x >= wRec.left && pt.x <= wRec.right && pt.y >= wRec.top && pt.y <= wRec.bottom) {

		}
	}*/
}
