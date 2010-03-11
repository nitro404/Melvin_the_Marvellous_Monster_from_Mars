#include "Menu.h"

Menu::Menu(const char * title,
		   int windowWidth,
		   int windowHeight,
		   D3DCOLOR menuTitleColour,
		   D3DCOLOR menuSelectionColour,
		   D3DCOLOR menuColour,
		   LPDIRECT3DDEVICE9 d3dDevice)
			: titleColour(menuTitleColour),
			  inactiveColour(menuColour),
			  activeColour(menuSelectionColour),
			  index(0),
			  menuItemOffset(250),
			  menuItemIncrement(55) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	// initialize the title text and message
	titleText = new Text("System", 58, Text::NORMAL, false, Text::CENTER, Text::CENTER, (int) (windowWidth / 2.0f), 100, titleColour, d3dDevice);

	this->title = new char[strlen(title) + 1];
	strcpy_s(this->title, strlen(title) + 1, title);
}

Menu::~Menu() {
	for(unsigned int i=0;i<menuItems.size();i++) {
		delete [] menuItems.at(i);
	}
	for(unsigned int i=0;i<menuItemText.size();i++) {
		delete menuItemText.at(i);
	}
	delete titleText;
	delete [] title;
}

void Menu::addMenuItem(char * menuItem, LPDIRECT3DDEVICE9 d3dDevice) {
	// create a text object for the current item
	menuItemText.push_back(new Text("System", 42, Text::BOLD, false, Text::CENTER, Text::CENTER, (int) (windowWidth / 2.0f), menuItemOffset + (menuItems.size() * menuItemIncrement), ((menuItems.size() == 0) ? activeColour : inactiveColour), d3dDevice));

	// store the message for the current item
	char * newMenuItem = new char[strlen(menuItem) + 1];
	strcpy_s(newMenuItem, strlen(menuItem) + 1, menuItem);
	menuItems.push_back(newMenuItem);
}

void Menu::moveUp() {
	// move the menu selection up (update colour on selected/previous object)
	menuItemText.at(index)->setColour(inactiveColour);
	index--;
	if(index < 0) { index = menuItems.size() - 1; }
	menuItemText.at(index)->setColour(activeColour);
}

void Menu::moveDown() {
	// move the menu selection down (update colour on selected/previous object)
	menuItemText.at(index)->setColour(inactiveColour);
	index++;
	if(index >= (int) menuItems.size()) { index = 0; }
	menuItemText.at(index)->setColour(activeColour);
}

int Menu::getIndex() {
	// get the current menu position
	return index;
}

void Menu::draw(LPDIRECT3DDEVICE9 d3dDevice) {
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	
	d3dDevice->BeginScene();

	// draw the menu title
	titleText->draw(title);

	// draw the menu items
	for(unsigned int i=0;i<menuItems.size();i++) {
		menuItemText.at(i)->draw(menuItems.at(i));
	}
	
	d3dDevice->EndScene();
	
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}
