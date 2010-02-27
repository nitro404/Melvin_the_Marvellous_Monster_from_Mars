#include "Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	
	Game game = Game(50, 50, 1024, 768, hInstance, WndProc, L"game", L"Melvin the Marvelous Monster from Mars", nCmdShow);
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