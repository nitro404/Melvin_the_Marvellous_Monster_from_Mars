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
			  movingAnimationSequence(0),
			  movingAnimationInterval(11),
			  movingAnimationEnd(movingAnimationInterval * 2),
			  movementDirection(0) {
	this->windowHeight = windowHeight;
	this->windowWidth = windowWidth;

	this->petSpriteSheet = spriteSheets->getSpriteSheet("Fluffy");
	this->petMovementSprites = new Sprite*[2];
	this->petMovementSprites[0] = this->petSpriteSheet->getSprite("Fluffy Idle 1");
	this->petMovementSprites[1] = this->petSpriteSheet->getSprite("Fluffy Idle 2");
	this->petSprite = this->petMovementSprites[0];

	this->scale = D3DXVECTOR2(1, 1);
	this->size = D3DXVECTOR2(petSprite->getWidth() * scale.x, petSprite->getHeight() * scale.y);
	this->offset = D3DXVECTOR2((petSprite->getWidth() / 2.0f) * scale.x, (petSprite->getHeight() / 2.0f) * scale.y);
	this->position = player.getBottomCenter();
	this->position.x -= offset.x;
	this->position.y -= size.y;
	this->velocity = D3DXVECTOR2(0, 0);
	this->velocityStep = 10.0f;

	setItemLimit(4);
}

Pet::~Pet() {
	delete [] petMovementSprites;
}

void Pet::tick() {
	if(player.hasFollowPosition()) {
		D3DXVECTOR2 newPosition = player.getFollowPosition();
		newPosition.x -= offset.x;
		newPosition.y -= size.y;
		if(!CollisionHandler::checkRadiusIntersection(player.getCenter(), getCenter(), 30, 30)) {
			if(newPosition.x != position.x) {
				movementDirection = (newPosition.x < position.x) ? -1 : 1;
			}

			position = newPosition;
			player.popFollowPosition();

			petSprite = petMovementSprites[(movingAnimationSequence / movingAnimationInterval)];

			movingAnimationSequence++;
			if(movingAnimationSequence >= movingAnimationEnd) {
				movingAnimationSequence = 0;
			}
		}
	}
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
