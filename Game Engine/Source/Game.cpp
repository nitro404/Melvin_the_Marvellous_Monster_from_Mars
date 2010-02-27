#include "Game.h"

Game::Game(int posX, int posY, int width, int height, HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow) {
	this->windowPosX = posX;
	this->windowPosY = posY;
	this->windowWidth = width;
	this->windowHeight = height;

	this->directInput = NULL;
	this->keyboard = NULL;
	this->mouse = NULL;

	this->d3d = NULL;
	this->d3dDev = NULL;

	if(init(hInstance, WndProc, winClassName, title, nCmdShow) != 0) {
		MessageBoxA(NULL, "Error initializing game.", "Error", MB_OK);
		PostQuitMessage(0);
	}
}

Game::~Game() {
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
	if(d3dDev != NULL) {
		d3dDev->Release();
	}
	if(d3d != NULL) {
		d3d->Release();
	}
}

void Game::tick() {
	processKeyboardInput();
	processMouseInput();
}

void Game::draw() {
	d3dDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	
	d3dDev->BeginScene();
	
	d3dDev->EndScene();
	
	d3dDev->Present(NULL, NULL, NULL, NULL);
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

int Game::init(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName, LPCTSTR title, int nCmdShow) {
	this->hInstance = hInstance;
	this->fps = 60;
	this->timePerFrame = 1000 / fps;
	
	if(RegisterWndClass(hInstance, WndProc, winClassName) != 0) {
		MessageBoxA(NULL, "Could not register class.", "Error", MB_OK);
		return 1;
	}
	
	hWnd = CreateWindow(winClassName, title, WS_OVERLAPPEDWINDOW, windowPosX, windowPosY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
	
	if(hWnd == NULL) {
		MessageBoxA(NULL, "Could not create window.", "Error", MB_OK);
		return 1;
	}
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	D3DPRESENT_PARAMETERS d3dpp;
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if(d3d == NULL) { return 1; }
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = windowWidth;
	d3dpp.BackBufferHeight = windowHeight;
	d3dpp.Windowed = true;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	if(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDev) != S_OK) {
		return 1;
	}
	
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **) &directInput, NULL);
	if(directInput == NULL) { return 1; }
	
	if(directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL) != S_OK) { return 1; }
	if(keyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK) { return 1; }
	if(keyboard->SetDataFormat(&c_dfDIKeyboard) != DI_OK) { return 1; }
	
	if(directInput->CreateDevice(GUID_SysMouse, &mouse, NULL) != DI_OK) { return 1; }
	if(mouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK) { return 1; }
	if(mouse->SetDataFormat(&c_dfDIMouse) != DI_OK) { return 1; }
	
	return 0;
}

int Game::RegisterWndClass(HINSTANCE hInstance, WNDPROC WndProc, LPCTSTR winClassName) {
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
		return 1;
	}
	
	return 0;
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
//		rocket->processKeys(DIK_LEFT);
	}

	if(keyboardState[DIK_RIGHT] & 0x80 || keyboardState[DIK_D] & 0x80) {
//		rocket->processKeys(DIK_RIGHT);
	}

	if(keyboardState[DIK_UP] & 0x80 || keyboardState[DIK_W] & 0x80) {
//		rocket->processKeys(DIK_UP);
	}

	if(keyboardState[DIK_DOWN] & 0x80 || keyboardState[DIK_S] & 0x80) {
//		rocket->processKeys(DIK_DOWN);
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
