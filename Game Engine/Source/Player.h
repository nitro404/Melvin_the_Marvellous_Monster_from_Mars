#pragma once

#include "Includes.h"
#include "DirectX.h"
#include "Point.h"

struct PlayerVertex {
    D3DXVECTOR3 pos;
	float rhw;
    DWORD colour;
};

#define D3DFVF_PLAYER_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
#define PLAYER_VERTEX_COUNT 5

class Player {
public:
	Player(int x, int y, int xBoundary, int yBoundary, LPDIRECT3DDEVICE9 d3dDevice);
	~Player();
	
	void tick();
	void Player::draw(LPDIRECT3DDEVICE9 d3dDevice);

	void moveLeft();
	void moveRight();
	void jump();
	void moveUp();
	void moveDown();

private:
	bool init(LPDIRECT3DDEVICE9 d3dDevice);

protected:
	PlayerVertex vertex[PLAYER_VERTEX_COUNT];
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	
private:
	static int movementSpeed;

	Point position;
	Point boundary;

	bool isJumping;
	float jumpTick;
	static float maxJumpHeight;
	
	LPDIRECT3DTEXTURE9 playerTexture;
	LPD3DXSPRITE playerSprite;
	D3DCOLOR playerColour;
};
