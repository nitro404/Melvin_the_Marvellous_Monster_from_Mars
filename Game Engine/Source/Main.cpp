#include "Main.h"

#if _DEBUG
#include <vld.h>
#include <crtdbg.h>
void checkForLeaks() {
	_CrtDumpMemoryLeaks();
}
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Variables * settings = new Variables();
	if (!settings->parseFrom("settings.ini")) {
		delete settings;
		quit("Error", "Could not find the settings file \"settings.ini\".");
	}
	
	Game game = Game(settings, hInstance, WndProc, "game", nCmdShow);
	game.run();
	
	#if _DEBUG
	atexit(checkForLeaks);
	#endif

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
