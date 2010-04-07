#include "Pet.h"
#include "Player.h"

Pet::Pet(float xPos,
		 float yPos,
		 int windowWidth,
		 int windowHeight,
		 Player & externalPlayer,
		 double & externalTimeElapsed,
		 Variables * externalSettings,
		 SpriteSheets * spriteSheets,
		 LPDIRECT3DDEVICE9 d3dDevice)
			: timeElapsed(externalTimeElapsed),
			  player(externalPlayer),
			  settings(externalSettings),
			  movementDirection(0) {
	this->windowHeight = windowHeight;
	this->windowWidth = windowWidth;

	this->petSpriteSheet = spriteSheets->getSpriteSheet("Fluffy");
	this->petSprite = petSpriteSheet->getSprite("Fluffy Idle 1");

	this->scale = D3DXVECTOR2(1, 1);
	this->size = D3DXVECTOR2(petSprite->getWidth() * scale.x, petSprite->getHeight() * scale.y);
	this->offset = D3DXVECTOR2((petSprite->getWidth() / 2.0f) * scale.x, (petSprite->getHeight() / 2.0f) * scale.y);
	this->position = D3DXVECTOR2(xPos - offset.x, yPos - offset.y);
	this->velocity = D3DXVECTOR2(0, 0);
	this->velocityStep = 10.0f;

	setItemLimit(4);
}

Pet::~Pet() {

}

void Pet::tick() {
//	setPosition(player.getX() - player.getOffsetX(), player.getY() - player.getOffsetY());
}

void Pet::draw(int * scrollingOffset, LPDIRECT3DDEVICE9 d3dDevice) {
	D3DXVECTOR2 offsetPosition = position;
	if(scrollingOffset != NULL) {
		offsetPosition.x -= (*scrollingOffset);
	}
	if(movementDirection >= 0) {
		petSprite->drawBackwards(&scale, &offset, 0, NULL, &offsetPosition, d3dDevice);
	}
	else {
		petSprite->draw(&scale, &offset, 0, NULL, &offsetPosition, d3dDevice);
	}
}
