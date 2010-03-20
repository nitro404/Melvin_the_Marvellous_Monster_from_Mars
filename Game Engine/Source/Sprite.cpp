#include "Sprite.h"

Sprite::Sprite(char * fileName, char * directory, LPDIRECT3DDEVICE9 d3dDevice) : texture(NULL), sprite(NULL) {
	if(fileName != NULL) {
		D3DXCreateSprite(d3dDevice, &sprite);

		USES_CONVERSION;
		wstring spriteFileName;
		if(directory != NULL) {
			spriteFileName.append(wstring(A2W(directory)));
			if(_stricmp(W2A(spriteFileName.substr(spriteFileName.length() - 1, spriteFileName.length()).c_str()), ("\\")) != 0 &&
			   _stricmp(W2A(spriteFileName.substr(spriteFileName.length() - 1, spriteFileName.length()).c_str()), ("/")) != 0) {
				spriteFileName.append(L"\\");
			}
		}
		spriteFileName.append(A2W(fileName));

		if(FAILED(D3DXCreateTextureFromFile(d3dDevice, spriteFileName.c_str(), &texture))) {
			quit("Error", "Error loading sprite texture \"%s\".", W2A(spriteFileName.c_str()));
		}

		D3DSURFACE_DESC description;
		texture->GetLevelDesc(0, &description);
		this->width = description.Width;
		this->height = description.Height;
	}
	else {
		this->width = 0;
		this->height = 0;
	}

	this->xOffset = -1;
	this->yOffset = -1;

	this->ownsSprite = true;
	
	D3DXMatrixScaling(&flipMatrix, -1, 1, 1);
}

Sprite::~Sprite() {
	if(ownsSprite) {
		if(texture != NULL) { texture->Release(); }
		if(sprite != NULL) { sprite->Release(); }
	}
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

#ifdef _DEBUG
		testDrawPoint(d3dDevice, position->x, position->y);
#endif

		sprite->End();
	}
}

void Sprite::drawBackwards(D3DXVECTOR2 * scale, D3DXVECTOR2 * scalingOffset, float rotationDegrees, D3DXVECTOR2 * rotationOffset, D3DXVECTOR2 * position, LPDIRECT3DDEVICE9 d3dDevice) {
	if(texture != NULL) {
		D3DXMatrixTransformation2D(&transformationMatrix, scalingOffset, 0, scale, rotationOffset, D3DXToRadian(rotationDegrees), position);
		D3DXMatrixMultiply(&flippedTransformationMatrix, &flipMatrix, &transformationMatrix);
		//D3DXMATRIX flipMatrixTranslation;
//		D3DXMatrixTransformation2D(&transformationMatrix, NULL, 0, NULL, NULL, 0, );

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
