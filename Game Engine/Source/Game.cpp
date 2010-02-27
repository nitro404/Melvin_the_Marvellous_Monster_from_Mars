#include "Game.h"

Game::Game(Variables * settings, HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow) {
	if(!verifySettings(settings)) {
		MessageBoxA(NULL, "Settings file is invalid.", "Error", MB_OK);
		PostQuitMessage(0);
	}

	this->windowPosX = atoi(settings->getValue("Window Position Horizontal"));
	this->windowPosY = atoi(settings->getValue("Window Position Vertical"));
	this->windowWidth = atoi(settings->getValue("Window Width"));
	this->windowHeight = atoi(settings->getValue("Window Height"));

	this->settings = settings;

	this->directInput = NULL;
	this->keyboard = NULL;
	this->mouse = NULL;

	this->d3d = NULL;
	this->d3dDevice = NULL;
	
	if(!init(hInstance, WndProc, winClassName, title, nCmdShow)) {
		MessageBoxA(NULL, "Error initializing game.", "Error", MB_OK);
		PostQuitMessage(0);
	}

	player = new Player(windowWidth/3,windowHeight-100, windowWidth, windowHeight, d3dDevice);
}

Game::~Game() {
	if(settings != NULL) { delete settings; }
	if(player != NULL) { delete player; }
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
		
		tick();
		draw();
		
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
	return	settings->hasValue("Window Width") &&
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
	
	if(keyboardState[DIK_LEFT] & 0x80 || keyboardState[DIK_A] & 0x80) {
		player->moveLeft();
	}
	
	if(keyboardState[DIK_RIGHT] & 0x80 || keyboardState[DIK_D] & 0x80) {
		player->moveRight();
	}
	
	if(keyboardState[DIK_UP] & 0x80 || keyboardState[DIK_W] & 0x80) {
		player->jump();
//		player->moveUp();
	}
	
	if(keyboardState[DIK_DOWN] & 0x80 || keyboardState[DIK_S] & 0x80) {
//		player->moveDown();
	}
	
	if(keyboardState[DIK_ESCAPE] & 0x80) {
		if(MessageBoxA(NULL, "Are you sure you want to quit?", "Quit", MB_YESNO) == IDYES) {
			PostQuitMessage(0);
		}
	}
}

void Game::processMouseInput() {
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
