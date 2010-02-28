#include "Player.h"

int Player::movementSpeed = 6;
float Player::maxJumpHeight = 27;

Player::Player(int x, int y, int xBoundary, int yBoundary, Variables * settings, LPDIRECT3DDEVICE9 d3dDevice) : vertexBuffer(NULL) {
	D3DXCreateSprite(d3dDevice, &playerSprite);
	
	USES_CONVERSION;
	spriteFileName = wstring(A2W(settings->getValue("Sprite Directory")));
	spriteFileName.append(L"\\Alien.PNG");

	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, spriteFileName.c_str(), &playerTexture))) {
		quit("Error", "Error loading player texture.");
	}
	
	D3DSURFACE_DESC description;
	playerTexture->GetLevelDesc(0, &description);
	spriteWidth = description.Width;
	spriteHeight = description.Height;

	this->boundary = Point(xBoundary - spriteWidth, yBoundary - spriteHeight);
	this->position = Point(boundary.x / 2.0f, boundary.y);
	this->isJumping = false;

	// initalize red box
	int i;
	VOID * pVtx;

	for(i=0;i<PLAYER_VERTEX_COUNT;i++) {
		vertex[i].pos.z = 0;
		vertex[i].rhw = 0.5;
		vertex[i].colour = D3DCOLOR_XRGB(255, 0, 0);
	}

	float xPos = 10;
	float yPos = 10;
	float sizeX = 10;
	float sizeY = 10;
	
	vertex[0].pos.x = (float) xPos - sizeX; vertex[0].pos.y = (float) yPos - sizeY;
	vertex[1].pos.x = (float) xPos + sizeX; vertex[1].pos.y = (float) yPos - sizeY;
	vertex[2].pos.x = (float) xPos + sizeX; vertex[2].pos.y = (float) yPos + sizeY;
	vertex[3].pos.x = (float) xPos + sizeX; vertex[3].pos.y = (float) yPos + sizeY;
	vertex[4].pos.x = (float) xPos - sizeX; vertex[4].pos.y = (float) yPos + sizeY;
	vertex[5].pos.x = (float) xPos - sizeX; vertex[5].pos.y = (float) yPos - sizeY;
	
	if(FAILED(d3dDevice->CreateVertexBuffer(PLAYER_VERTEX_COUNT * sizeof(PlayerVertex), 0, D3DFVF_PLAYER_VERTEX, D3DPOOL_DEFAULT, &vertexBuffer, NULL))) { quit("Error", "Error creating vertex buffer for player."); }
	if(FAILED(vertexBuffer->Lock(0, sizeof(PlayerVertex) * PLAYER_VERTEX_COUNT, (void **) &pVtx, 0))) { quit("Error", "Error locking vertex buffer for player."); }
	memcpy(pVtx, vertex, sizeof(PlayerVertex) * PLAYER_VERTEX_COUNT);
	vertexBuffer->Unlock();
}

Player::~Player() {
	if(playerTexture != NULL) { playerTexture->Release(); }
	if(playerSprite != NULL) { playerSprite->Release(); }

	if(vertexBuffer != NULL) {
		vertexBuffer->Release();
	}
}

void Player::tick() {
	if(isJumping) {
		position.y = boundary.y - (-(jumpTick * jumpTick)) - maxJumpHeight;
		jumpTick += 0.3f;
		if(position.y > boundary.y) {
			isJumping = false;
			position.y = boundary.y;
		}
	}
	playerColour = D3DCOLOR_RGBA(255, 255, 255, 255);
}

void Player::draw(LPDIRECT3DDEVICE9 d3dDevice) {
	D3DXMATRIX transformation;
	D3DXVECTOR2 translation = D3DXVECTOR2((float) position.x, (float) position.y);
	D3DXVECTOR2 scaling(1, 1);

	D3DXMatrixTransformation2D(&transformation, NULL, 0, &scaling, NULL, 0, &translation);
	playerSprite->SetTransform(&transformation);

	playerSprite->Begin(D3DXSPRITE_ALPHABLEND);

	playerSprite->Draw(playerTexture, NULL, NULL, NULL, playerColour);

	playerSprite->End();

	// draw red box
	int i;
	PlayerVertex * pVtx;

	D3DXMATRIX matTransform;
	D3DXMatrixTranslation(&matTransform, (float) position.x, (float) position.y, 1);

	if(FAILED(vertexBuffer->Lock(0, sizeof(PlayerVertex) * PLAYER_VERTEX_COUNT, (void **) &pVtx, 0))) { return; }
	memcpy(pVtx, vertex, sizeof(PlayerVertex) * PLAYER_VERTEX_COUNT);
	for(i=0;i<PLAYER_VERTEX_COUNT;i++) { D3DXVec3TransformCoord(&pVtx[i].pos, &vertex[i].pos, &matTransform); }
	vertexBuffer->Unlock();
	
	d3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(PlayerVertex) );
	d3dDevice->SetFVF(D3DFVF_PLAYER_VERTEX);
	d3dDevice->SetRenderState(D3DRS_COLORVERTEX, FALSE);
	d3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 3);
}

void Player::moveLeft() {
	if(position.x - movementSpeed < 0) { position.x = 0; }
	else { position.x -= movementSpeed; }
}

void Player::moveRight() {
	if(position.x + movementSpeed > boundary.x) { position.x = boundary.x; }
	else { position.x += movementSpeed; }
}

void Player::jump() {
	if(!isJumping) {
		isJumping = true;
		jumpTick = -5.15f;
	}
}

void Player::moveUp() {
	if(position.y - movementSpeed < 0) { position.y = 0; }
	else { position.y -= movementSpeed; }
}

void Player::moveDown() {
	if(position.y + movementSpeed > boundary.y) { position.y = boundary.y; }
	else { position.y += movementSpeed; }
}
