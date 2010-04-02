#include "Game.h"
#include "Menu.h"

const int Menu::MENU_MAIN = 0;
const int Menu::MENU_NEWGAME = 1;
const int Menu::MENU_HELP = 2;

Menu::Menu(int windowWidth,
		   int windowHeight,
		   Game * externalGame,
		   Variables * settings,
		   LPDIRECT3DDEVICE9 d3dDevice)
			: game(externalGame),
			  titleColour(D3DCOLOR_RGBA(0, 255, 0, 255)),
			  activeColour(D3DCOLOR_RGBA(40, 90, 255, 255)),
			  inactiveColour(D3DCOLOR_RGBA(255, 255, 255, 255)),
			  mainMenuIndex(0),
			  mapMenuIndex(0),
			  titleOffset(50),
			  menuItemOffset(225),
			  menuItemIncrement(55),
			  menuType(MENU_MAIN),
			  active(true) {
	if(game == NULL) {
		quit("Error", "Menu must have a game to control, game cannot be null.");
	}
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	loadMapList(settings->getValue("Map Directory"));

	titleFont = new Font("System", 96, Font::NORMAL, false, d3dDevice);
	itemFont = new Font("System", 52, Font::BOLD, false, d3dDevice);

	int menuItemPosition = menuItemOffset;
	mainMenuTitle = new Text((int) (windowWidth / 2.0f), titleOffset, Text::CENTER, Text::CENTER, titleColour, titleFont, true, d3dDevice, "Melvin the Marvellous");
	mainMenuTitle2 = new Text((int) (windowWidth / 2.0f), titleOffset + 70, Text::CENTER, Text::CENTER, titleColour, titleFont, true, d3dDevice, "Monster from Mars");
	mainMenuItems.push_back(new Text((int) (windowWidth / 2.0f), menuItemPosition, Text::CENTER, Text::CENTER, activeColour, itemFont, true, d3dDevice, "New Game"));
	mainMenuItems.push_back(new Text((int) (windowWidth / 2.0f), menuItemPosition += menuItemIncrement, Text::CENTER, Text::CENTER, inactiveColour, itemFont, true, d3dDevice, "Help"));
	mainMenuItems.push_back(new Text((int) (windowWidth / 2.0f), menuItemPosition += menuItemIncrement, Text::CENTER, Text::CENTER, inactiveColour, itemFont, true, d3dDevice, "Quit"));

	mapMenuTitle = new Text((int) (windowWidth / 2.0f), titleOffset, Text::CENTER, Text::CENTER, titleColour, titleFont, true, d3dDevice, "Choose Level");
	for(unsigned int i=0;i<mapList.size();i++) {
		char * mapNameWithExt = strchr((char *) mapList.at(i).c_str(), '\\') + sizeof(char);
		char * mapName = new char[strlen(mapNameWithExt) + 1];
		strcpy_s(mapName, strlen(mapNameWithExt) + 1, mapNameWithExt);
		char * mapExt = strrchr(mapName, '.');
		*mapExt = '\0';
		mapMenuItems.push_back(new Text((int) (windowWidth / 2.0f), menuItemOffset + (menuItemIncrement * i), Text::CENTER, Text::CENTER, ((i == 0) ? activeColour : inactiveColour), itemFont, true, d3dDevice, mapName));
		delete [] mapName;
	}

	menuItemPosition = menuItemOffset;
	helpMenuTitle = new Text((int) (windowWidth / 2.0f), titleOffset, Text::CENTER, Text::CENTER, titleColour, titleFont, true, d3dDevice, "Help");
	helpMenuItems.push_back(new Text((int) (windowWidth / 2.0f),  menuItemOffset, Text::CENTER, Text::CENTER, inactiveColour, itemFont, true, d3dDevice, "Use WSAD or arrows to move."));
	helpMenuItems.push_back(new Text((int) (windowWidth / 2.0f),  menuItemOffset += menuItemIncrement, Text::CENTER, Text::CENTER, inactiveColour, itemFont, true, d3dDevice, "Press Space to jump and G to grab items."));
}

Menu::~Menu() {
	delete mainMenuTitle;
	delete mainMenuTitle2;
	for(unsigned int i=0;i<mainMenuItems.size();i++) {
		delete mainMenuItems.at(i);
	}
	delete mapMenuTitle;
	for(unsigned int i=0;i<mapMenuItems.size();i++) {
		delete mapMenuItems.at(i);
	}
	delete helpMenuTitle;
	for(unsigned int i=0;i<helpMenuItems.size();i++) {
		delete helpMenuItems.at(i);
	}
	delete titleFont;
	delete itemFont;
}

void Menu::select() {
	if(!active) { return; }
	
	if(menuType == MENU_MAIN) {
		if(mainMenuIndex == 0) {
			setMenu(MENU_NEWGAME);
		}
		else if(mainMenuIndex == 1) {
			setMenu(MENU_HELP);
		}
		else if(mainMenuIndex == 2) {
			PostQuitMessage(0);
		}
	}
	else if(menuType == MENU_NEWGAME) {
		game->loadLevel(mapList.at(mapMenuIndex).c_str());
		resumeGame();
		setMenu(MENU_MAIN);
	}
	else if(menuType == MENU_HELP) {
		setMenu(MENU_MAIN);
	}
}

void Menu::back() {
	if(!active) {
		setMenu(MENU_MAIN);
		pauseGame();
	}
	else {
		if(menuType == 0) {
//			setMenu(MENU_MAIN);
//			resumeGame();
			PostQuitMessage(0);
		}
		else if(menuType == 1 || menuType == 2) {
			setMenu(MENU_MAIN);
		}
	}
}

void Menu::moveUp() {
	// move the menu selection up (update colour on selected/previous object)
	if(menuType == MENU_MAIN) {
		mainMenuItems.at(mainMenuIndex)->setColour(inactiveColour);
		mainMenuIndex--;
		if(mainMenuIndex < 0) { mainMenuIndex = mainMenuItems.size() - 1; }
		mainMenuItems.at(mainMenuIndex)->setColour(activeColour);
	}
	else if(menuType == MENU_NEWGAME) {
		mapMenuItems.at(mainMenuIndex)->setColour(inactiveColour);
		mapMenuIndex--;
		if(mapMenuIndex < 0) { mapMenuIndex = mapMenuItems.size() - 1; }
		mapMenuItems.at(mainMenuIndex)->setColour(activeColour);
	}
}

void Menu::moveDown() {
	// move the menu selection down (update colour on selected/previous object)
	if(menuType == MENU_MAIN) {
		mainMenuItems.at(mainMenuIndex)->setColour(inactiveColour);
		mainMenuIndex++;
		if(mainMenuIndex >= (int) mainMenuItems.size()) { mainMenuIndex = 0; }
		mainMenuItems.at(mainMenuIndex)->setColour(activeColour);
	}
	else if(menuType == MENU_NEWGAME) {
		mapMenuItems.at(mapMenuIndex)->setColour(inactiveColour);
		mapMenuIndex++;
		if(mapMenuIndex >= (int) mapMenuItems.size()) { mapMenuIndex = 0; }
		mapMenuItems.at(mapMenuIndex)->setColour(activeColour);
	}
}

bool Menu::isActive() { return active; }
void Menu::activate() { active = true; }
void Menu::deactivate() { active = false; }
void Menu::toggle() { active = !active; }

void Menu::loadMapList(const char * mapDirectory) {
	if(mapDirectory == NULL || strlen(mapDirectory) == 0) { return; }

	string rootDirectory = mapDirectory;
	string filePath;
	string strPattern;
	string strFileName;
	HANDLE hFile;
	WIN32_FIND_DATA fileInformation;

	strPattern = rootDirectory + "\\*.2d";

	hFile = ::FindFirstFile(strPattern.c_str(), & fileInformation);
	if(hFile != INVALID_HANDLE_VALUE) {
		do {
			if(fileInformation.cFileName[0] != '.') {
				filePath.erase();
				filePath = rootDirectory + "\\" + fileInformation.cFileName;

				if(!(fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					strFileName = fileInformation.cFileName;
					mapList.push_back(filePath);
				}
			}
		} while(::FindNextFile(hFile, &fileInformation) == TRUE);

		::FindClose(hFile);
	}
}

void Menu::draw(LPDIRECT3DDEVICE9 d3dDevice) {
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	
	d3dDevice->BeginScene();

	if(menuType == MENU_MAIN) {
		// draw the menu title
		mainMenuTitle->draw();
		mainMenuTitle2->draw();

		// draw the menu items
		for(unsigned int i=0;i<mainMenuItems.size();i++) {
			mainMenuItems.at(i)->draw();
		}
	}
	else if(menuType == MENU_NEWGAME) {
		// draw the menu title
		mapMenuTitle->draw();

		// draw the menu items
		for(unsigned int i=0;i<mapMenuItems.size();i++) {
			mapMenuItems.at(i)->draw();
		}
	}
	else if(menuType == MENU_HELP) {
		// draw the menu title
		helpMenuTitle->draw();

		// draw the menu items
		for(unsigned int i=0;i<helpMenuItems.size();i++) {
			helpMenuItems.at(i)->draw();
		}
	}
	
	d3dDevice->EndScene();
	
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Menu::setMenu(int type) {
	menuType = type;
	/*
	vector<Text *> * items;
	if(type == MENU_MAIN) { items = &mainMenuItems; }
	else if(type == MENU_NEWGAME) { items = &mapMenuItems; }
	else if(type == MENU_HELP) { return; }
	else { return; }
	for(unsigned int i=0;i<items->size();i++) {
		items->at(i)->setColour((i == 0) ? activeColour : inactiveColour);
	}*/
}

void Menu::pauseGame() {
//	game->pause();
	active = true;
}

void Menu::resumeGame() {
	/*
	game->resume();
	if(!game->isPaused()) {
		active = false;
	}
	*/
	active = false;
}
