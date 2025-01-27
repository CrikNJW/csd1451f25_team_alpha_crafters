#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "Structs.hpp"

//Marcos for the trigo functions that take input in degree 
//cuz alpha engine only takes input in radians for function sin, cos, tan
#define AESinDeg(x) AESin(AEDegToRad(x));
#define AECosDeg(x) AECos(AEDegToRad(x));

//------------------ square mesh ------------------------------

AEGfxVertexList* createSquareMesh() {
	u32 white = 0xFFFFFFFF;
	
	AEGfxMeshStart();
	// Draw square mesh
	AEGfxTriAdd(
		-0.5f, -0.5f, white, 0.0f, 1.0f,  // bottom-left: white
		0.5f, -0.5f, white, 1.0f, 1.0f,   // bottom-right: white
		-0.5f, 0.5f, white, 0.0f, 0.0f);  // top-left: white

	AEGfxTriAdd(
		0.5f, -0.5f, white, 1.0f, 1.0f,   // bottom-right: white
		0.5f, 0.5f, white, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, white, 0.0f, 0.0f);  // top-left: white

	// Saving the mesh (list of triangles) in mesh
	return AEGfxMeshEnd();
	////END OF MESH
}

void InitializePlatform(Platform& platform) {

	AEMtx33 scaleMtx, translateMtx;
	AEMtx33Scale(&scaleMtx, platform.width, platform.height);
	AEMtx33Trans(&translateMtx, platform.PosX, platform.PosY);
	AEMtx33Concat(&platform.finalTransform, &translateMtx, &scaleMtx);
}

void RenderPlatform(Platform& platform, AEGfxVertexList* mesh) {

	AEGfxSetColorToMultiply(0.0f, 1.0f, 0.0f, 1.0f); // Green color
	AEGfxSetTransform(platform.finalTransform.m); // Apply precomputed transformation
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

void UpdateGroundEnemy(Ground_enemy& enemy, Platform& platform, float dt) {
	
	float platformLeft = platform.PosX - (platform.width / 2) - (enemy.width / 2);
	float platformRight = platform.PosX + (platform.width / 2) + (enemy.width / 2);
	float platformTop = platform.PosY + (platform.height / 2) + (enemy.width / 2);
	float platformBottom = platform.PosY - (platform.height / 2) - (enemy.width / 2);

	switch (enemy.state) {
	case MOVE_RIGHT:
		enemy.PosX += enemy.speed * dt;
		if (enemy.PosX >= platformRight) {
			enemy.PosX = platformRight;  // Stop at edge
			enemy.state = MOVE_DOWN;     // Change direction
		}
		break;

	case MOVE_DOWN:
		enemy.PosY -= enemy.speed * dt;
		if (enemy.PosY <= platformBottom) {
			enemy.PosY = platformBottom;
			enemy.state = MOVE_LEFT;
		}
		break;

	case MOVE_LEFT:
		enemy.PosX -= enemy.speed * dt;
		if (enemy.PosX <= platformLeft) {
			enemy.PosX = platformLeft;
			enemy.state = MOVE_UP;
		}
		break;

	case MOVE_UP:
		enemy.PosY += enemy.speed * dt;
		if (enemy.PosY >= platformTop) {
			enemy.PosY = platformTop;
			enemy.state = MOVE_RIGHT;
		}
		break;
	}

	AEMtx33 scaleMtx, translateMtx;
	AEMtx33Scale(&scaleMtx, enemy.width, enemy.height);
	AEMtx33Trans(&translateMtx, enemy.PosX, enemy.PosY);
	AEMtx33Concat(&enemy.finalTransform, &translateMtx, &scaleMtx);
}
	
void RenderGroundEnemy(Ground_enemy& enemy, AEGfxVertexList* mesh) {
	// Set enemy color
	AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f); // Red color

	// Apply transformation matrix
	AEGfxSetTransform(enemy.finalTransform.m);

	// Draw enemy
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

