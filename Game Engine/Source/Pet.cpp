// ======================================= //
// Melvin the Marvellous Monster from Mars //
//                                         //
// Author: Kevin Scroggins                 //
// E-Mail: nitro404@hotmail.com            //
// Date: April 11, 2010                    //
// ======================================= //

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

	// load the sprites corresponding to the pet
	this->petSpriteSheet = spriteSheets->getSpriteSheet("Fluffy");
	this->petMovementSprites = new Sprite*[2];
	this->petMovementSprites[0] = this->petSpriteSheet->getSprite("Fluffy Idle 1");
	this->petMovementSprites[1] = this->petSpriteSheet->getSprite("Fluffy Idle 2");
	this->petSprite = this->petMovementSprites[0];

	// initialize the transformation variables
	this->scale = D3DXVECTOR2(1, 1);
	this->size = D3DXVECTOR2(petSprite->getWidth() * scale.x, petSprite->getHeight() * scale.y);
	this->offset = D3DXVECTOR2((petSprite->getWidth() / 2.0f) * scale.x, (petSprite->getHeight() / 2.0f) * scale.y);
	this->position = player.getBottomCenter();
	this->position.x -= offset.x;
	this->position.y -= size.y;
	this->velocity = D3DXVECTOR2(0, 0);
	this->velocityStep = 10.0f;

	// set limit of the number of items the pet can carry to 4
	setItemLimit(4);
}

Pet::~Pet() {
	delete [] petMovementSprites;
}

// update the pet
void Pet::tick() {
	// if the player has moved at least once
	if(player.hasFollowPosition()) {
		// obtain the position the player last moved to and offset it
		D3DXVECTOR2 newPosition = player.getFollowPosition();
		newPosition.x -= offset.x;
		newPosition.y -= size.y;

		// check if the pet is colliding with the player and if not
		if(!CollisionHandler::checkRadiusIntersection(player.getCenter(), getCenter(), 30, 30)) {
			// if the positions are not the same, update the movement direction
			if(newPosition.x != position.x) {
				movementDirection = (newPosition.x < position.x) ? -1 : 1;
			}

			// set the pet's position to the new position and remove this position from the player
			position = newPosition;
			player.popFollowPosition();

			// load the appropriate pet animation sprite
			petSprite = petMovementSprites[(movingAnimationSequence / movingAnimationInterval)];

			// increment the pet movement animation
			movingAnimationSequence++;
			if(movingAnimationSequence >= movingAnimationEnd) {
				movingAnimationSequence = 0;
			}
		}
	}
}

// render the pet
void Pet::draw(int * scrollingOffset, LPDIRECT3DDEVICE9 d3dDevice) {
	// apply the sidescrolling offset
	D3DXVECTOR2 offsetPosition = position;
	if(scrollingOffset != NULL) {
		offsetPosition.x -= (*scrollingOffset);
	}

	// render the pet facing the direction it is currently moving
	if(movementDirection >= 0) {
		petSprite->drawBackwards(&scale, &offset, 0, NULL, &offsetPosition, d3dDevice);
	}
	else {
		petSprite->draw(&scale, &offset, 0, NULL, &offsetPosition, d3dDevice);
	}
}
