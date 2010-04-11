// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

#include "Sprite.h"

// sprite type constants
int Sprite::TYPE_UNKNOWN = -1;
int Sprite::TYPE_SHEET = 0;
int Sprite::TYPE_TILE = 1;
int Sprite::TYPE_OBJECT = 2;
int Sprite::TYPE_PLAYER = 3;
int Sprite::TYPE_DISGUISE = 4;
int Sprite::TYPE_PET = 5;
int Sprite::TYPE_AI = 6;
int Sprite::TYPE_ITEM = 6;

// constructor for creating a sprite from a given file name and sprite directory
Sprite::Sprite(const char * fileName,
			   const char * directory,
			   LPDIRECT3DDEVICE9 d3dDevice)
				: texture(NULL),
				  sprite(NULL),
				  name(NULL),
				  parentName(NULL),
				  index(-1),
				  type(TYPE_UNKNOWN),
				  xOffset(-1),
				  yOffset(-1),
				  width(0),
				  height(0),
				  ownsSprite(true) {
	if(fileName != NULL) {
		D3DXCreateSprite(d3dDevice, &sprite);

		string spriteFileName;
		if(directory != NULL) {
			spriteFileName.append(directory);
			if(_stricmp(spriteFileName.substr(spriteFileName.length() - 1, spriteFileName.length()).c_str(), ("\\")) != 0 &&
			   _stricmp(spriteFileName.substr(spriteFileName.length() - 1, spriteFileName.length()).c_str(), ("/")) != 0) {
				spriteFileName.append("\\");
			}
		}
		spriteFileName.append(fileName);


		if(FAILED(D3DXCreateTextureFromFile(d3dDevice, spriteFileName.c_str(), &texture))) {
			quit("Error", "Error loading sprite texture \"%s\".", spriteFileName.c_str());
		}

		D3DSURFACE_DESC description;
		texture->GetLevelDesc(0, &description);
		this->width = description.Width;
		this->height = description.Height;
	}
	
	D3DXMatrixScaling(&flipMatrix, -1, 1, 1);
}

// constructor for creating a subsprite from an already existing sprite's directx texture and sprite
Sprite::Sprite(int x,
			   int y,
			   int spriteWidth,
			   int spriteHeight,
			   LPDIRECT3DTEXTURE9 externalTexture,
			   LPD3DXSPRITE externalSprite)
				: xOffset(x),
				  yOffset(y),
				  name(NULL),
				  parentName(NULL),
				  index(-1),
				  type(TYPE_UNKNOWN),
				  width(spriteWidth),
				  height(spriteHeight),
				  texture(externalTexture),
				  sprite(externalSprite),
				  ownsSprite(false) { D3DXMatrixScaling(&flipMatrix, -1, 1, 1); }

// destructor
Sprite::~Sprite() {
	if(ownsSprite) {
		if(texture != NULL) { texture->Release(); }
		if(sprite != NULL) { sprite->Release(); }
	}
	if(name != NULL) { delete [] name; }
	if(parentName != NULL) { delete [] parentName; }
}

// get the sprite's x offset (width / 2)
int Sprite::getOffsetX() const { return xOffset; }

// get the sprite's y offset (height / 2)
int Sprite::getOffsetY() const { return yOffset; }

// get the sprite's width
int Sprite::getWidth() const { return width; }

// get the sprite's height
int Sprite::getHeight() const { return height; }

// check if the sprite is tiled
bool Sprite::isTiled() { return type == TYPE_TILE; }

// get the sprite's name
char * Sprite::getName() const { return this->name; }

// get the name of the sprite sheet which the sprite belongs to
char * Sprite::getParentName() const { return this->parentName; }

// get the index of the sprite within its parent sprite sheet
int Sprite::getIndex() const { return this->index; }

// get the type of the sprite
int Sprite::getType() const { return this->type; }

// set the name of the sprite
void Sprite::setName(char * name) {
	if(name == NULL) { return; }
	if(this->name != NULL) {
		delete [] this->name;
	}
	this->name = new char[strlen(name) + 1];
	strcpy_s(this->name, strlen(name) + 1, name);
}

// set the name of the sprite sheet which the sprite belongs to
void Sprite::setParentName(char * parentName) {
	if(parentName == NULL) {
		this->parentName = NULL;
		return;
	}
	if(this->parentName != NULL) {
		delete [] this->parentName;
	}
	this->parentName = new char[strlen(parentName) + 1];
	strcpy_s(this->parentName, strlen(parentName) + 1, parentName);
}

// set the index of the sprite in its parent sprite sheet
void Sprite::setIndex(int index) {
	this->index = (index < -1) ? -1 : index;
}

// set the type of the sprite
void Sprite::setType(int type) {
	if(type == TYPE_SHEET ||
	   type == TYPE_TILE ||
	   type == TYPE_OBJECT ||
	   type == TYPE_PLAYER ||
	   type == TYPE_DISGUISE ||
	   type == TYPE_PET ||
	   type == TYPE_AI ||
	   type == TYPE_ITEM) {
		this->type = type;
	}
	else {
		this->type = TYPE_UNKNOWN;
	}
}

// get the sprite's directx texture handle
LPDIRECT3DTEXTURE9 Sprite::getTexture() const { return texture; }

// get the sprite's directx sprite handle
LPD3DXSPRITE Sprite::getSprite() const { return sprite; }

// parse the sprite's type from a string and return it
int Sprite::parseType(const char * data) {
	int type = TYPE_UNKNOWN;
	if(data == NULL) { return type; }
	char * typeString = strtrimcpy(data);
	
	if(_stricmp(typeString, "SpriteSheet") == 0) {
		type = TYPE_SHEET;
	}
	else if(_stricmp(typeString, "Tile") == 0) {
		type = TYPE_TILE;
	}
	else if(_stricmp(typeString, "Object") == 0) {
		type = TYPE_OBJECT;
	}
	else if(_stricmp(typeString, "Player") == 0) {
		type = TYPE_PLAYER;
	}
	else if(_stricmp(typeString, "Disguise") == 0) {
		type = TYPE_DISGUISE;
	}
	else if(_stricmp(typeString, "Pet") == 0) {
		type = TYPE_PET;
	}
	else if(_stricmp(typeString, "AI") == 0) {
		type = TYPE_AI;
	}
	else if(_stricmp(typeString, "Item") == 0) {
		type = TYPE_ITEM;
	}
	delete [] typeString;
	return type;
}

// render the sprite using a pre-computed matrix
void Sprite::draw(D3DXMATRIX & transformationMatrix, LPDIRECT3DDEVICE9 d3dDevice) {
	if(texture != NULL) {
		// apply the transformation to the sprite
		sprite->SetTransform(&transformationMatrix);

		sprite->Begin(D3DXSPRITE_ALPHABLEND);

		// if the sprite is in its natural state, simply render it
		if(xOffset < 0 && yOffset < 0) {
			sprite->Draw(texture, NULL, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}
		// otherwise render the sprite within a bounding box (a sub-sprite)
		else {
			RECT spriteRect = {xOffset, yOffset, xOffset + width, yOffset + width};
			sprite->Draw(texture, &spriteRect, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}

		sprite->End();
	}
}

// render the sprite using a pre-defined collection of transformation attributes
void Sprite::draw(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice) {
	if(texture != NULL) {
		// compute the transformation to be applied to the sprite
		D3DXMatrixTransformation2D(&transformationMatrix, scalingOffset, 0, scale, rotationOffset, D3DXToRadian(rotationDegrees), position);

		// apply the transformation to the sprite
		sprite->SetTransform(&transformationMatrix);

		sprite->Begin(D3DXSPRITE_ALPHABLEND);

		// if the sprite is in its natural state, simply render it
		if(xOffset < 0 && yOffset < 0) {
			sprite->Draw(texture, NULL, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}
		// otherwise render the sprite within a bounding box (a sub-sprite)
		else {
			RECT spriteRect = {xOffset, yOffset, xOffset + width, yOffset + height};
			sprite->Draw(texture, &spriteRect, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}

		sprite->End();
	}
}

// render the sprite backwards using a pre-defined set of transformation attributes
void Sprite::drawBackwards(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice) {
	if(texture != NULL) {
		// compute the transformation to be applied to the sprite and then flip the transformation along the x axis
		D3DXVECTOR2 flippedPosition(position->x + (scalingOffset->x * 2), position->y);
		D3DXMatrixTransformation2D(&transformationMatrix, scalingOffset, 0, scale, rotationOffset, D3DXToRadian(rotationDegrees), &flippedPosition);
		D3DXMatrixMultiply(&flippedTransformationMatrix, &flipMatrix, &transformationMatrix);

		// apply the transformation to the sprite
		sprite->SetTransform(&flippedTransformationMatrix);

		sprite->Begin(D3DXSPRITE_ALPHABLEND);

		// if the sprite is in its natural state, simply render it
		if(xOffset < 0 && yOffset < 0) {
			sprite->Draw(texture, NULL, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}
		// otherwise render the sprite within a bounding box (a sub-sprite)
		else {
			RECT spriteRect = {xOffset, yOffset, xOffset + width, yOffset + height};
			sprite->Draw(texture, &spriteRect, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}

		sprite->End();
	}
}
