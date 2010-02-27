#ifndef _INCLUDES_H
#define _INCLUDES_H

#include <windows.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

#define DIRECTINPUT_VERSION 0x0800

#include <d3d9.h>
#include <d3dX9.h>
#include <dinput.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

void prompt(char * message, ...);

#endif
