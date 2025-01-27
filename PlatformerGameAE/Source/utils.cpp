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
	AEMtx33Scale(&scaleMtx, platform.Width, platform.Height);
	AEMtx33Trans(&translateMtx, platform.PosX, platform.PosY);
	AEMtx33Concat(&platform.finalTransform, &translateMtx, &scaleMtx);
}

void RenderPlatform(Platform& platform, AEGfxVertexList* mesh) {

	AEGfxSetColorToMultiply(0.0f, 1.0f, 0.0f, 1.0f); // Green color
	AEGfxSetTransform(platform.finalTransform.m); // Apply precomputed transformation
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

void UpdateGroundEnemy(Ground_enemy& enemy, Platform& platform, float dt) {
	
	float platformLeft = platform.PosX - (platform.Width / 2) - (enemy.Width / 2);
	float platformRight = platform.PosX + (platform.Width / 2) + (enemy.Width / 2);
	float platformTop = platform.PosY + (platform.Width / 2) + (enemy.Width / 2);
	float platformBottom = platform.PosY - (platform.Width / 2) - (enemy.Width / 2);

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

<<<<<<< HEAD
	

// player default WSAD controls
void UpdatePlayerMovement(Player *player , AEGfxVertexList* player_mesh) {
	player->speed = AEFrameRateControllerGetFrameTime() * 300.f; //speed of player according to frame rate
	//player movement 
	if (AEInputCheckCurr(AEVK_W)) player->posY -= player->speed;
	if (AEInputCheckCurr(AEVK_S)) player->posY += player->speed;
	if (AEInputCheckCurr(AEVK_A)) player->posX += player->speed;
	if (AEInputCheckCurr(AEVK_D)) player->posX -= player->speed;
	
}

//collision for player and boundary 
void CheckCollision(Player& player, const Boundaries& boundary) {

	// Calculate bounds
	float playerLeft = player.posX - player.width / 2;
	float playerRight = player.posX + player.width / 2;
	float playerBottom = player.posY - player.height / 2;
	float playerTop = player.posY + player.height / 2;

	float boundaryLeft = boundary.PosX - boundary.Width / 2;
	float boundaryRight = boundary.PosX + boundary.Width / 2;
	float boundaryBottom = boundary.PosY - boundary.Height / 2;
	float boundaryTop = boundary.PosY + boundary.Height / 2;

	// Check if there's a collision
	if (playerLeft < boundaryRight && playerRight > boundaryLeft &&
		playerBottom < boundaryTop && playerTop > boundaryBottom) {

		// Calculate overlap in each direction
		float overlapLeft = boundaryRight - playerLeft;
		float overlapRight = playerRight - boundaryLeft;
		float overlapTop = playerTop - boundaryBottom;
		float overlapBottom = boundaryTop - playerBottom;

		float minOverlap = overlapLeft;
		if (overlapRight < minOverlap) minOverlap = overlapRight;
		if (overlapTop < minOverlap) minOverlap = overlapTop;
		if (overlapBottom < minOverlap) minOverlap = overlapBottom;

		// Resolve collision by moving the player out of the boundary
		if (minOverlap == overlapLeft) {
			player.posX += overlapLeft;
		}
		else if (minOverlap == overlapRight) {
			player.posX -= overlapRight;
		}
		else if (minOverlap == overlapTop) {
			player.posY -= overlapTop;
		}
		else if (minOverlap == overlapBottom) {
			player.posY += overlapBottom;
		}
	}
}

// for bounce collision between player and enemy
void ElasticEnemyCollision(Player& player, Ground_enemy& enemy) {
	// Calculate the edges of the centered enemy rectangle
	float enemyLeft = enemy.PosX - enemy.Width / 2;
	float enemyRight = enemy.PosX + enemy.Width / 2;
	float enemyTop = enemy.PosY - enemy.Height / 2;
	float enemyBottom = enemy.PosY + enemy.Height / 2;

	// Calculate the edges of the player rectangle 
	float playerLeft = player.posX - player.width / 2;
	float playerRight = player.posX + player.width / 2;
	float playerTop = player.posY - player.height / 2;
	float playerBottom = player.posY + player.height / 2;

	// Check for collision
	if (playerRight > enemyLeft && playerLeft < enemyRight &&
		playerBottom > enemyTop && playerTop < enemyBottom) {

		// Bounce back distance 
		float bounceBackDistance = 40.0f; // Adjust this value to control bounce intensity

		float moveDirectionMultiplier = 1.0f;
		if (AEInputCheckCurr(AEVK_S)) { // only for moving backwards
			// If moving backwards, reverse the bounce direction
			moveDirectionMultiplier = -1.0f;
		}

		// Bounce the player back in the opposite direction of current movement
		player.posX -= moveDirectionMultiplier * bounceBackDistance * AECos(AEDegToRad(player.rotate_angle));
		player.posY -= moveDirectionMultiplier * bounceBackDistance * AESin(AEDegToRad(player.rotate_angle));

		
	}
}
=======
>>>>>>> parent of c81e7e0 (Merge branch 'main' into Trisha-GroundEnemyAI)
