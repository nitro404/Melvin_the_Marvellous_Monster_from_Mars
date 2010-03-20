#pragma once

#include "Includes.h"
#include "DirectX.h"

class Sprite {
public:
	Sprite(char * fileName, char * directory, LPDIRECT3DDEVICE9 d3dDevice);
	Sprite::Sprite(int x,
				   int y,
				   int spriteWidth,
				   int spriteHeight,
				   LPDIRECT3DTEXTURE9 externalTexture,
				   LPD3DXSPRITE externalSprite)
					: xOffset(x),
					  yOffset(y),
					  width(spriteWidth),
					  height(spriteHeight),
					  texture(externalTexture),
					  sprite(externalSprite),
					  ownsSprite(false) { D3DXMatrixScaling(&flipMatrix, -1, 1, 1); }
	~Sprite();

	int getOffsetX() { return xOffset; }
	int getOffsetY() { return yOffset; }

	int getWidth() { return width; }
	int getHeight() { return height; }

	LPDIRECT3DTEXTURE9 getTexture() { return texture; }
	LPD3DXSPRITE getSprite() { return sprite; }

	void draw(D3DXMATRIX & transformation, LPDIRECT3DDEVICE9 d3dDevice);
	void draw(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice);
	void drawCentered(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice);
	void drawBackwards(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice);
	void drawBackwardsCentered(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice);

private:
	LPDIRECT3DTEXTURE9 texture;
	LPD3DXSPRITE sprite;
	bool ownsSprite;

	D3DXMATRIX transformationMatrix;
	D3DXMATRIX flipMatrix;
	D3DXMATRIX flippedTransformationMatrix;

	int xOffset;
	int yOffset;

	int width;
	int height;
};
