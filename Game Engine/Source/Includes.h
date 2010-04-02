#pragma once

#include <windows.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <atlbase.h>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

#include "Constants.h"
#include "DirectX.h"

#if _DEBUG
	void testDrawPoint(LPDIRECT3DDEVICE9 d3dDevice, float x, float y);
	void testDrawLine(LPDIRECT3DDEVICE9 d3dDevice, float x1, float y1, float x2, float y2);
	void testDrawEmptyBox(LPDIRECT3DDEVICE9 d3dDevice, float x, float y, float r);
	void testDrawEmptyBox(LPDIRECT3DDEVICE9 d3dDevice, float x, float y, float rx, float ry);
	void testDrawEmptyCircle(LPDIRECT3DDEVICE9 d3dDevice, float x, float y, float xr, float yr);
#endif

char * strtrimcpy(const char * data);
bool isTrue(const char * str);
void quit(const char * title, const char * message, ...);
void prompt(const char * message, ...);
