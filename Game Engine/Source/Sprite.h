#pragma once

#include "Includes.h"
#include "DirectX.h"

class Sprite {
public:
	Sprite(const char * fileName, const char * directory, LPDIRECT3DDEVICE9 d3dDevice);
	Sprite(int x, int y, int spriteWidth, int spriteHeight, LPDIRECT3DTEXTURE9 externalTexture, LPD3DXSPRITE externalSprite);
	~Sprite();

	int getOffsetX();
	int getOffsetY();

	int getWidth();
	int getHeight();

	bool isTiled();

	char * getName();
	char * getParentName();
	int getIndex();
	int getType();

	void setName(char * name);
	void setParentName(char * parentName);
	void setIndex(int index);
	void setType(int type);

	LPDIRECT3DTEXTURE9 getTexture() { return texture; }
	LPD3DXSPRITE getSprite() { return sprite; }

	static int parseType(const char * data);

	void draw(D3DXMATRIX & transformation, LPDIRECT3DDEVICE9 d3dDevice);
	void draw(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice);
	void drawBackwards(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice);

public:
	static int TYPE_UNKNOWN;
	static int TYPE_SHEET;
	static int TYPE_TILE;
	static int TYPE_OBJECT;
	static int TYPE_PLAYER;
	static int TYPE_DISGUISE;
	static int TYPE_PET;
	static int TYPE_AI;
	static int TYPE_ITEM;

private:
	LPDIRECT3DTEXTURE9 texture;
	LPD3DXSPRITE sprite;
	bool ownsSprite;

	D3DXMATRIX transformationMatrix;
	D3DXMATRIX flipMatrix;
	D3DXMATRIX flippedTransformationMatrix;

	char * name;
	char * parentName;
	int index;
	int type;

	int xOffset;
	int yOffset;

	int width;
	int height;
};
