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
#include <deque>

using namespace std;

#include "Constants.h"
#include "DirectX.h"

#if _DEBUG
extern D3DCOLOR _DEBUG_COLOUR;
void testDrawPoint(LPDIRECT3DDEVICE9 d3dDevice, float x, float y, D3DCOLOR colour, int * scrollingOffset);
void testDrawLine(LPDIRECT3DDEVICE9 d3dDevice, float x1, float y1, float x2, float y2, D3DCOLOR colour, int * scrollingOffset);
void testDrawEmptyBox(LPDIRECT3DDEVICE9 d3dDevice, float x, float y, float r, D3DCOLOR colour, int * scrollingOffset);
void testDrawEmptyBox(LPDIRECT3DDEVICE9 d3dDevice, float x, float y, float rx, float ry, D3DCOLOR colour, int * scrollingOffset);
void testDrawBox(LPDIRECT3DDEVICE9 d3dDevice, float x, float y, float r, D3DCOLOR colour, int * scrollingOffset);
void testDrawBox(LPDIRECT3DDEVICE9 d3dDevice, float x, float y, float xr, float yr, D3DCOLOR colour, int * scrollingOffset);
void testDrawEmptyCircle(LPDIRECT3DDEVICE9 d3dDevice, float x, float y, float xr, float yr, D3DCOLOR colour, int * scrollingOffset);
#endif

char * strtrimcpy(const char * data);
bool isTrue(const char * str);
void quit(const char * title, const char * message, ...);
void prompt(const char * message, ...);
