#include "Sprite.h"

int Sprite::TYPE_UNKNOWN = -1;
int Sprite::TYPE_SHEET = 0;
int Sprite::TYPE_TILE = 1;
int Sprite::TYPE_OBJECT = 2;
int Sprite::TYPE_PLAYER = 3;
int Sprite::TYPE_DISGUISE = 4;
int Sprite::TYPE_PET = 5;
int Sprite::TYPE_AI = 6;

Sprite::Sprite(char * fileName,
			   char * directory,
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

Sprite::~Sprite() {
	if(ownsSprite) {
		if(texture != NULL) { texture->Release(); }
		if(sprite != NULL) { sprite->Release(); }
	}
	if(name != NULL) { delete [] name; }
	if(parentName != NULL) { delete [] parentName; }
}

int Sprite::getOffsetX() { return xOffset; }
int Sprite::getOffsetY() { return yOffset; }

int Sprite::getWidth() { return width; }
int Sprite::getHeight() { return height; }

bool Sprite::isTiled() { return type == TYPE_TILE; }

char * Sprite::getName() { return this->name; }

char * Sprite::getParentName() { return this->parentName; }

int Sprite::getIndex() { return this->index; }

int Sprite::getType() { return this->type; }

void Sprite::setName(char * name) {
	if(name == NULL) { return; }
	if(this->name != NULL) {
		delete [] this->name;
	}
	this->name = new char[strlen(name) + 1];
	strcpy_s(this->name, strlen(name) + 1, name);
}

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

void Sprite::setIndex(int index) {
	this->index = (index < -1) ? -1 : index;
}

void Sprite::setType(int type) {
	if(type == TYPE_SHEET ||
	   type == TYPE_TILE ||
	   type == TYPE_OBJECT ||
	   type == TYPE_PLAYER ||
	   type == TYPE_DISGUISE ||
	   type == TYPE_PET ||
	   type == TYPE_AI) {
		this->type = type;
	}
	else {
		this->type = TYPE_UNKNOWN;
	}
}

int Sprite::parseType(const char * data) {
	if(data == NULL) { return TYPE_UNKNOWN; }
	char * typeString = strtrimcpy(data);
	int type = TYPE_UNKNOWN;
	
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
	delete [] typeString;
	return type;
}

void Sprite::draw(D3DXMATRIX & transformationMatrix, LPDIRECT3DDEVICE9 d3dDevice) {
	if(texture != NULL) {
		sprite->SetTransform(&transformationMatrix);

		sprite->Begin(D3DXSPRITE_ALPHABLEND);

		if(xOffset < 0 && yOffset < 0) {
			sprite->Draw(texture, NULL, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}
		else {
			RECT spriteRect = {xOffset, yOffset, xOffset + width, yOffset + width};
			sprite->Draw(texture, &spriteRect, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}

		sprite->End();
	}
}

void Sprite::draw(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice) {
	if(texture != NULL) {
		D3DXMatrixTransformation2D(&transformationMatrix, scalingOffset, 0, scale, rotationOffset, D3DXToRadian(rotationDegrees), position);

		sprite->SetTransform(&transformationMatrix);

		sprite->Begin(D3DXSPRITE_ALPHABLEND);

		if(xOffset < 0 && yOffset < 0) {
			sprite->Draw(texture, NULL, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}
		else {
			RECT spriteRect = {xOffset, yOffset, xOffset + width, yOffset + height};
			sprite->Draw(texture, &spriteRect, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}

		sprite->End();
	}
}

void Sprite::drawCentered(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice) {
	if(texture != NULL) {
		D3DXVECTOR2 center(position->x - scalingOffset->x, position->y - scalingOffset->y);
		D3DXMatrixTransformation2D(&transformationMatrix, scalingOffset, 0, scale, rotationOffset, D3DXToRadian(rotationDegrees), &center);

		sprite->SetTransform(&transformationMatrix);

		sprite->Begin(D3DXSPRITE_ALPHABLEND);

		if(xOffset < 0 && yOffset < 0) {
			sprite->Draw(texture, NULL, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}
		else {
			RECT spriteRect = {xOffset, yOffset, xOffset + width, yOffset + height};
			sprite->Draw(texture, &spriteRect, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}

		sprite->End();
	}
}

void Sprite::drawBackwards(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice) {
	if(texture != NULL) {
		D3DXVECTOR2 flippedPosition(position->x + (scalingOffset->x * 2), position->y);
		D3DXMatrixTransformation2D(&transformationMatrix, scalingOffset, 0, scale, rotationOffset, D3DXToRadian(rotationDegrees), &flippedPosition);
		D3DXMatrixMultiply(&flippedTransformationMatrix, &flipMatrix, &transformationMatrix);

		sprite->SetTransform(&flippedTransformationMatrix);

		sprite->Begin(D3DXSPRITE_ALPHABLEND);

		if(xOffset < 0 && yOffset < 0) {
			sprite->Draw(texture, NULL, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}
		else {
			RECT spriteRect = {xOffset, yOffset, xOffset + width, yOffset + height};
			sprite->Draw(texture, &spriteRect, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}

		sprite->End();
	}
}

void Sprite::drawBackwardsCentered(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice) {
	if(texture != NULL) {
		D3DXVECTOR2 flippedCenter(position->x + scalingOffset->x, position->y - scalingOffset->y);
		D3DXMatrixTransformation2D(&transformationMatrix, scalingOffset, 0, scale, rotationOffset, D3DXToRadian(rotationDegrees), &flippedCenter);
		D3DXMatrixMultiply(&flippedTransformationMatrix, &flipMatrix, &transformationMatrix);

		sprite->SetTransform(&flippedTransformationMatrix);

		sprite->Begin(D3DXSPRITE_ALPHABLEND);

		if(xOffset < 0 && yOffset < 0) {
			sprite->Draw(texture, NULL, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}
		else {
			RECT spriteRect = {xOffset, yOffset, xOffset + width, yOffset + height};
			sprite->Draw(texture, &spriteRect, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
		}

		sprite->End();
	}
}
