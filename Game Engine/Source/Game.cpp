#include "Game.h"

Game::Game(Variables * settings,
		   HINSTANCE hInstance,
		   WNDPROC WndProc,
		   LPCTSTR winClassName,
		   LPCTSTR title,
		   int nCmdShow)
		   : mainMenu(NULL),
			 mainMenuActive(true),
			 helpItemOffset(250),
			 helpItemIncrement(55),
			 helpScreenActive(false),
			 spaceBarPressed(false),
			 menuUpKeyPressed(false),
			 menuDownKeyPressed(false),
			 menuSelectKeyPressed(false),
			 escapeKeyPressed(false),
			 directInput(NULL),
			 keyboard(NULL),
			 mouse(NULL),
			 d3d(NULL),
			 d3dDevice(NULL) {
	this->settings = settings;
	if(!verifySettings(settings)) {
		quit("Error", "Settings file is invalid.");
	}
	
	this->windowPosX = atoi(settings->getValue("Window Position Horizontal"));
	this->windowPosY = atoi(settings->getValue("Window Position Vertical"));
	this->windowWidth = atoi(settings->getValue("Window Width"));
	this->windowHeight = atoi(settings->getValue("Window Height"));
	
	if(!init(hInstance, WndProc, winClassName, title, nCmdShow)) {
		quit("Error", "Error initializing game.");
	}

	player = new Player(windowWidth / 2.0f, (float) windowHeight, windowWidth, windowHeight, settings, d3dDevice);

	// create the main menu
	mainMenu = new Menu("Melvin the Marvellous Monster from Mars", windowWidth, windowHeight, D3DCOLOR_RGBA(0, 255, 0, 255), D3DCOLOR_RGBA(0, 255, 0, 255), D3DCOLOR_RGBA(0, 170, 0, 255), d3dDevice);
	mainMenu->addMenuItem("New Game", d3dDevice);
	mainMenu->addMenuItem("Load Game", d3dDevice);
	mainMenu->addMenuItem("Help", d3dDevice);
	mainMenu->addMenuItem("Quit", d3dDevice);

	// create the help screen
	helpTitleText = new Text("System", 58, Text::BOLD, false, Text::CENTER, Text::CENTER, (int) (windowWidth / 2.0f), 100, D3DCOLOR_RGBA(0, 255, 0, 255), d3dDevice);
	for(unsigned int i=0;i<helpItemMessages.size();i++) {
		helpItemText.push_back(new Text("System", 42, Text::BOLD, false, Text::CENTER, Text::CENTER, (int) (windowWidth / 2.0f),  helpItemOffset + (i * helpItemIncrement), D3DCOLOR_RGBA(0, 170, 0, 255), d3dDevice));
	}
}

Game::~Game() {
	delete settings;
	delete player;
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

void Game::tick() {
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

void Game::drawHelp() {
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	
	d3dDevice->BeginScene();

	// display help title
	helpTitleText->draw("Help");

	// display help items
	for(unsigned int i=0;i<helpItemText.size();i++) {
		helpItemText.at(i)->draw(helpItemMessages.at(i));
	}

	d3dDevice->EndScene();
	
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Game::reset() { }

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
		
		if(mainMenuActive) {
			processKeyboardInput();

			if(helpScreenActive) {
				drawHelp();
			}
			else {
				mainMenu->draw(d3dDevice);
			}
			
		}
		// otherwise, render the game
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

bool loadMap(char * fileName) {
	if(fileName == NULL || strlen(fileName) == 0) { return false; }

	ifstream in(fileName);
	if(in.bad()) { return false; }

	const int MAX_STRING_LENGTH = 1024;
	char input[MAX_STRING_LENGTH];
	
	in.getline(input, MAX_STRING_LENGTH);
	
	if(in.is_open()) { in.close(); }

	return true;
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

	if(keyboardState[DIK_ESCAPE] & 0x80) {
		if(!escapeKeyPressed) {
			if(helpScreenActive) {
				helpScreenActive = false;
			}
			else if(!mainMenuActive) {
				mainMenuActive = true;
			}
			else {
				PostQuitMessage(0);
			}
		}
		escapeKeyPressed = true;
	}
	else {
		escapeKeyPressed = false;
	}

	processMainMenuInput();

	processPlayerInput();
}

void Game::processMainMenuInput() {
	if(helpScreenActive || !mainMenuActive) { return; }

	if(keyboardState[DIK_RETURN] & 0x80 || keyboardState[DIK_SPACE] & 0x80) {
		if(!menuSelectKeyPressed) {
			switch(mainMenu->getIndex()) {
				// start game in single player mode
				case 0:
					mainMenuActive = false;
					break;
				// start game in 2 player mode
				case 1:
					mainMenuActive = false;
					break;
				// display help screen
				case 2:
					helpScreenActive = true;
					break;
				// quit the game
				case 3:
					PostQuitMessage(0);
					break;
			}
		}
		menuSelectKeyPressed = true;
	}
	else { menuSelectKeyPressed = false; }

	// move menu selection up
	if(keyboardState[DIK_UP] & 0x80 || keyboardState[DIK_W] & 0x80) {
		if(!menuUpKeyPressed) {
			mainMenu->moveUp();
		}
		menuUpKeyPressed = true;
	}
	else { menuUpKeyPressed = false; }

	// move menu selection down
	if(keyboardState[DIK_DOWN] & 0x80 || keyboardState[DIK_S] & 0x80) {
		if(!menuDownKeyPressed) {
			mainMenu->moveDown();
		}
		menuDownKeyPressed = true;
	}
	else { menuDownKeyPressed = false; }
}

void Game::processPlayerInput() {
	if(mainMenuActive || helpScreenActive) { return; }

	if(keyboardState[DIK_LEFT] & 0x80 || keyboardState[DIK_A] & 0x80) {
		player->moveLeft();
		player->isMoving = -1;
	}
	
	if(keyboardState[DIK_RIGHT] & 0x80 || keyboardState[DIK_D] & 0x80) {
		player->moveRight();
		player->isMoving = 1;
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

	POINT pt;
	RECT wRec;

	if(mouseState.rgbButtons[0] & 0x80) {
		GetCursorPos(&pt); 
		ScreenToClient(hWnd, &pt);
		GetWindowRect(hWnd, &wRec); 

		if(pt.x >= wRec.left && pt.x <= wRec.right && pt.y >= wRec.top && pt.y <= wRec.bottom) {
//			this->rocket->setNewHeading(pt.x, pt.y);
		}
	}
}
