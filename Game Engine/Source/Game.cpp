#include "Game.h"

float Game::gravity = 0.98f;

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
			 directInput(NULL),
			 keyboard(NULL),
			 mouse(NULL),
			 d3d(NULL),
			 d3dDevice(NULL),
			 timeElapsed(0) {
	this->settings = settings;
	if(!verifySettings(settings)) {
		quit("Error", "Settings file is invalid.");
	}
	
	this->windowPosX = atoi(settings->getValue("Window Position Horizontal"));
	this->windowPosY = atoi(settings->getValue("Window Position Vertical"));
	this->windowWidth = atoi(settings->getValue("Window Width"));
	this->windowHeight = atoi(settings->getValue("Window Height"));
	
	if(!init(hInstance, WndProc, winClassName, "Melvin the Marvellous Monster from Mars", nCmdShow)) {
		quit("Error", "Error initializing game.");
	}

	spriteSheets = SpriteSheets::parseFrom(settings->getValue("SpriteSheet File"), settings->getValue("Sprite Directory"), d3dDevice);

	player = new Player(windowWidth / 2.0f, (float) windowHeight, windowWidth, windowHeight, timeElapsed, settings, spriteSheets, d3dDevice);

	menu = new Menu(windowWidth, windowHeight, this, settings, d3dDevice);
}

Game::~Game() {
	delete settings;
	if(spriteSheets != NULL) { delete spriteSheets; }
	delete player;
	if(level != NULL) { delete level; }
	delete menu;
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

void Game::tick() {
	computeTimeElapsed();

	processKeyboardInput();
	processMouseInput();
	
	player->tick();
}

void Game::draw() {
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(173, 244, 255), 1.0f, 0);
	
	d3dDevice->BeginScene();

	player->draw(d3dDevice);
	
	d3dDevice->EndScene();
	
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Game::reset() {
	if(level != NULL) {
		
	}
}

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
		
		if(menu->isActive()) {
			processKeyboardInput();

			menu->draw(d3dDevice);
		}
		else {
			tick();

			draw();
		}

		elapsedTime = (DWORD) (GetTickCount() - lastRenderTime);
		if(elapsedTime < timePerFrame) {
			Sleep((DWORD) (timePerFrame - elapsedTime));
		}
		lastRenderTime = GetTickCount();
    }
	
    return msg.wParam;
}

void Game::closeLevel() {
	if(level != NULL) {
		delete level;
		level = NULL;
	}
}

void Game::loadLevel(const char * fileName) {
	closeLevel();
	level = new Level(fileName, spriteSheets);
	reset();
}

bool Game::verifySettings(Variables * settings) {
	return	settings->hasValue("Sprite Directory") &&
			settings->hasValue("Window Width") &&
			settings->hasValue("Window Height");
}

bool Game::init(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow) {
	this->hInstance = hInstance;
	this->fps = 60;
	this->timePerFrame = 1000 / fps;
	
	if(!RegisterWndClass(hInstance, WndProc, winClassName)) {
		MessageBoxA(NULL, "Could not register class.", "Error", MB_OK);
		return false;
	}
	
	hWnd = CreateWindow(winClassName, title, WS_OVERLAPPEDWINDOW, windowPosX, windowPosY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
	
	if(hWnd == NULL) {
		MessageBoxA(NULL, "Could not create window.", "Error", MB_OK);
		return false;
	}
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
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
	
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **) &directInput, NULL);
	if(directInput == NULL) { return false; }
	
	if(directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL) != S_OK) { return false; }
	if(keyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK) { return false; }
	if(keyboard->SetDataFormat(&c_dfDIKeyboard) != DI_OK) { return false; }
	
	if(directInput->CreateDevice(GUID_SysMouse, &mouse, NULL) != DI_OK) { return false; }
	if(mouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK) { return false; }
	if(mouse->SetDataFormat(&c_dfDIMouse) != DI_OK) { return false; }

	return true;
}

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

void Game::processKeyboardInput() {
	if(!GetFocus()) { return; }

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

	processMenuInput();

	processPlayerInput();
}

void Game::processMenuInput() {
	// go back in the menu (or quit the game / open the menu, depending)
	if(keyboardState[DIK_ESCAPE] & 0x80) {
		if(!menuBackKeyPressed) { menu->back(); }
		menuBackKeyPressed = true;
	}
	else { menuBackKeyPressed = false; }

	if(!menu->isActive()) { return; }

	// select the current item in the menu
	if(keyboardState[DIK_RETURN] & 0x80 || keyboardState[DIK_SPACE] & 0x80) {
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
	if(menu->isActive()) { return; }

	player->isMoving = false;
	if(keyboardState[DIK_LEFT] & 0x80 || keyboardState[DIK_A] & 0x80) {
		player->moveLeft();
		player->isMoving = true;
	}
	
	if(keyboardState[DIK_RIGHT] & 0x80 || keyboardState[DIK_D] & 0x80) {
		player->moveRight();
		player->isMoving = true;
	}
	
	if(keyboardState[DIK_UP] & 0x80 || keyboardState[DIK_W] & 0x80) {
		player->jump();
	}
}

void Game::processMouseInput() {
	if(!GetFocus()) { return; }

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
