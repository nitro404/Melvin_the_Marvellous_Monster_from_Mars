// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#pragma once

#include "Includes.h"
#include "DirectX.h"

// the sprite class represents a 2d image which can be rendered to the screen with a specific transformation
// can also be used to store a sprite sheet image within a spritesheet object
class Sprite {
public:
	// constructor for creating a sprite from a given file name and sprite directory
	Sprite(const char * fileName, const char * directory, LPDIRECT3DDEVICE9 d3dDevice);
	// constructor for creating a subsprite from an already existing sprite's directx texture and sprite
	Sprite(int x, int y, int spriteWidth, int spriteHeight, LPDIRECT3DTEXTURE9 externalTexture, LPD3DXSPRITE externalSprite);
	// destructor
	~Sprite();

	int getOffsetX() const; // get the sprite's x offset (width / 2)
	int getOffsetY() const; // get the sprite's y offset (height / 2)

	int getWidth() const; // get the sprite's width
	int getHeight() const; // get the sprite's height

	bool isTiled(); // check if the sprite is tiled

	char * getName() const; // get the sprite's name
	char * getParentName() const; // get the name of the sprite sheet which the sprite belongs to
	int getIndex() const; // get the index of the sprite within its parent sprite sheet
	int getType() const; // get the type of the sprite

	void setName(char * name); // set the name of the sprite
	void setParentName(char * parentName); // set the name of the sprite sheet which the sprite belongs to
	void setIndex(int index); // set the index of the sprite in its parent sprite sheet
	void setType(int type); // set the type of the sprite

	LPDIRECT3DTEXTURE9 getTexture() const; // get the sprite's directx texture handle
	LPD3DXSPRITE getSprite() const; // get the sprite's directx sprite handle

	static int parseType(const char * data); // parse the sprite's type from a string and return it

	// render the sprite using a pre-computed matrix
	void draw(D3DXMATRIX & transformation, LPDIRECT3DDEVICE9 d3dDevice);
	// render the sprite using a pre-defined collection of transformation attributes
	void draw(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice);
	// render the sprite backwards using a pre-defined set of transformation attributes
	void drawBackwards(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice);

public:
	// sprite type constants
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
	// directx texture and sprite handles
	LPDIRECT3DTEXTURE9 texture;
	LPD3DXSPRITE sprite;
	bool ownsSprite;

	// pre-defined transformation matrices
	D3DXMATRIX transformationMatrix;
	D3DXMATRIX flipMatrix;
	D3DXMATRIX flippedTransformationMatrix;

	// sprite attributes
	char * name;
	char * parentName;
	int index;
	int type;

	// sprite offset and size
	int xOffset;
	int yOffset;
	int width;
	int height;
};
