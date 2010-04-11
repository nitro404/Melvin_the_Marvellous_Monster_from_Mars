// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#include "Main.h"

#if _DEBUG
// memory leak checking (debug mode only)
#include <vld.h>
#include <crtdbg.h>
void checkForLeaks() {
	_CrtDumpMemoryLeaks();
}
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	// load the configuration settings for the game and pass them to the game
	Variables * settings = new Variables();
	if (!settings->parseFrom("settings.ini")) {
		delete settings;
		quit("Error", "Could not find the settings file \"settings.ini\".");
	}
	
	// instantiate and execute the game object
	Game game = Game(settings, hInstance, WndProc, "game", nCmdShow);
	game.run();
	
#if _DEBUG
	// memory leak checking (debug mode only)
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
