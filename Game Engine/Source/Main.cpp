#include "Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Variables * settings = new Variables();
	if (!settings->parseFrom("settings.ini")) {
		delete settings;
		MessageBoxA(NULL, "Could not find the settings file \"settings.ini\".", "Error", MB_OK);
		PostQuitMessage(0);
	}
	
	Game game = Game(settings, hInstance, WndProc, L"game", L"Melvin the Marvelous Monster from Mars", nCmdShow);
	game.run();
	
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) {
        case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
            break;
    }
	
    return DefWindowProc(hWnd, message, wParam, lParam);
}