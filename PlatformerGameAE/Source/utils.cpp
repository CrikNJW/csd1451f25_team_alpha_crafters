#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "Structs.hpp"

//Marcos for the trigo functions that take input in degree 
//cuz alpha engine only takes input in radians for function sin, cos, tan
//#define AESinDeg(x) AESin(AEDegToRad(x));
//#define AECosDeg(x) AESin(AEDegToRad(x));

/*Creates a white square mesh, drawing from the center outwards in every direction, this is done to preserve the center coordinate*/
AEGfxVertexList* createSquareMesh() {
	u32 white = 0xFFFFFFFF;
	u32 red = 0xFFFF0000;
	
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

AEGfxVertexList* createCircleMesh() {
	AEGfxMeshStart();
	//Add the vertices for the circle
	for (int i = 0; i < 36; ++i) {
		//We want 36 triangles, 10 degrees per angle
		//cos and sin funcs only accept radians. 360 degrees = 2PI radians, therefore we multiply them together
		f32 CircleAngleInRadians = 2 * PI;
		f32 AnglePerTriangle = CircleAngleInRadians / 36;

		//We need to calculate 2 vertexes at the circumference of the circle
		f32 angle1 = i * AnglePerTriangle; //This is for the first vertex
		f32 angle2 = (i + 1) * AnglePerTriangle; //This is for the second vertex.

		//The cosf and sinf gives us the x,y coordinates.
		//We multiply it by the radius of the circle we have set.
		f32 x1 = 1 * cosf(angle1);
		f32 y1 = 1 * sinf(angle1);
		f32 x2 = 1 * cosf(angle2);
		f32 y2 = 1 * sinf(angle2);

		//UV coordinates are NORMALIZED, so it is from 0 to 1
		AEGfxTriAdd(
			0.0f, 0.0f, 0xFFFFFFFF, 0.5f, 0.5f, // Center of the circle, UV is 0.5, 0.5
			x1, y1, 0xFFFFFFFF, 0.5f + 0.5f * cosf(angle1), 0.5f + 0.5f * sinf(angle1), //First vertex, UV is calculated by the cos and sin of the angle
			x2, y2, 0xFFFFFFFF, 0.5f + 0.5f * cosf(angle2), 0.5f + 0.5f * sinf(angle2) //Second vertex, UV is calculated by the cos and sin of the angle
		);
	}

	// End creating the mesh and save it
	return AEGfxMeshEnd();
}

/*Creates a 3x3 transformation matrix based on the scale, rotate and translate parameters provided.*/
AEMtx33 createTransformMtx(f32 scaleX, f32 scaleY, f32 rotate_rad, f32 translX, f32 translY) {
	// Create a scale matrix that scales by 500 x and y
	AEMtx33 scaleMtx = { 0 };
	AEMtx33Scale(&scaleMtx, scaleX, scaleY);
	// Create a rotation matrix that rotates by 90 degrees
	// Note that PI in radians is 180 degrees.
	// Since 90 degrees is 180/2, 90 degrees in radians is PI/2
	AEMtx33 rotateMtx = { 0 };
	AEMtx33Rot(&rotateMtx, rotate_rad);
	// Create a translation matrix that translates by
	// 200 in the x-axis and 100 in the y-axis
	AEMtx33 translateMtx = { 0 };
	AEMtx33Trans(&translateMtx, translX, translY);
	// Concatenate the matrices into the 'transform' variable.
	// We concatenate in the order of translation * rotation * scale
	// i.e. this means we scale, then rotate, then translate.
	AEMtx33 transformMtx = { 0 };
	AEMtx33Concat(&transformMtx, &rotateMtx, &scaleMtx);
	AEMtx33Concat(&transformMtx, &translateMtx, &transformMtx);

	return transformMtx;
}

//int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
//{
//	// Reason why use half width and height is because the rectangle button was drew from the middle
//	float half_width = area_width / 2.0;
//	float half_height = area_height / 2.0;
//
//	// Compute the boundaries of the button rectangle
//	float left_bound = area_center_x - half_width;
//	float right_bound = area_center_x + half_width;
//	float top_bound = area_center_y - half_height;
//	float bottom_bound = area_center_y + half_height;
//
//	// Check if the Mouse_Input is within the boundaries
//	if (click_x >= left_bound && click_x <= right_bound && click_y >= top_bound && click_y <= bottom_bound) {
//		return 1;
//	}
//	else {
//		return 0;
//	}
//}

//int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
//{
//	float radius = diameter / 2.0;
//	// distance = sgrt of [(X1 - X2)^2 + (Y1 - Y2)^2], we eliminate the sqrt by squaring the radius later
//	float distance_squared = (click_x - circle_center_x) * (click_x - circle_center_x) + (click_y - circle_center_y) * (click_y - circle_center_y);
//
//	if (distance_squared <= radius * radius) {
//		return 1;
//	}
//	else {
//		return 0;
//	}
//}

int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2)
{
	float collide_dist_sqr = (c1_x - c2_x) * (c1_x - c2_x) + (c1_y - c2_y) * (c1_y - c2_y);

	if (collide_dist_sqr <= (r1 + r2) * (r1 + r2)) {
		return 1;
	}
	else {
		return 0;
	}
}


void DrawBlackOverlay(AEGfxVertexList* square_mesh) {
	f32 rec_width = AEGfxGetWindowWidth();
	f32 rec_height = AEGfxGetWindowHeight();
	AEMtx33 black_overlayMtx = createTransformMtx(rec_width,rec_height,0,0,0);

	//Dim the black colour rectangle
	AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.1f);
	AEGfxSetTransform(black_overlayMtx.m);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
}

void SpotLight(Player* player, AEGfxVertexList* circle_mesh) {
	f32 radius = 200.f;
	AEMtx33 spotlightMtx = createTransformMtx(radius, radius, 0, player->posX, player->posY);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 0.3f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransform(spotlightMtx.m);
	AEGfxMeshDraw(circle_mesh, AE_GFX_MDM_TRIANGLES);
	
}


void UpdatePlayerPos(Player *player, AEGfxVertexList* player_mesh, f32 dt) {
	if (player->lockMovement == true) {
		player->lockTimeElapsed += dt;
		if (player->lockTimeElapsed >= 2.0f) {
			player->lockMovement = false;
			player->lockTimeElapsed = 0;
		}
	}
	else {
		player->speed = f32(AEFrameRateControllerGetFrameTime() * 300.f); //speed of player according to frame rate
		f32 rotate_degree = 4.f; //rotation degree is set to 2 degree when trigerred

		if (AEInputCheckCurr(AEVK_W)) {
			//alpha engine only takes input in radians for function sin, cos, tan
			//Needs to manually convert the degree to radians
			player->posY += player->speed * AESin(AEDegToRad(player->rotate_angle));
			player->posX += player->speed * AECos(AEDegToRad(player->rotate_angle));
		}
		else if (AEInputCheckCurr(AEVK_S)) {
			player->posY -= player->speed * AESin(AEDegToRad(player->rotate_angle));
			player->posX -= player->speed * AECos(AEDegToRad(player->rotate_angle));
		}


		if (AEInputCheckCurr(AEVK_LEFT) || AEInputCheckCurr(AEVK_A)) {
			player->rotate_angle += rotate_degree;
			if (player->rotate_angle >= 360.f) {
				player->rotate_angle = 0.f;
			}
		}
		else if (AEInputCheckCurr(AEVK_RIGHT) || AEInputCheckCurr(AEVK_D)) {
			player->rotate_angle -= rotate_degree;
			if (player->rotate_angle < 0) {
				player->rotate_angle = 360.f;
			}
		}
	}

	//Draw the player Mesh
	AEMtx33 playerMtx = createTransformMtx(player->width, player->height, AEDegToRad(player->rotate_angle), player->posX, player->posY);
	AEGfxSetTransform(playerMtx.m);
	AEGfxMeshDraw(player_mesh, AE_GFX_MDM_TRIANGLES);
}

//Draw icicle at the given position
void DrawIcicle(f32 posX, f32 posY , AEGfxVertexList* icicleMesh) {
	//AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 0.1f); // Icicle Colour (blue)
	AEMtx33 icicleMtx = createTransformMtx(30.0f, 30.0f, 0, posX, posY);
	AEGfxSetTransform(icicleMtx.m);
	AEGfxMeshDraw(icicleMesh, AE_GFX_MDM_TRIANGLES);
}

//Draw icicle child and make it repeatedly drop icicles.
void Draw_UpdateIcicleDrop(Icicle &icicle, AEGfxVertexList* icicleMesh, f32 dt) {
	//AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 0.0f); // Icicle Drop Colour (blue)
	if (icicle.cooldownElapsed < icicle.cooldown) {
		icicle.cooldownElapsed += dt;
	}

	else {
		icicle.childY -= icicle.dropSpeed * dt;
		icicle.timeElapsed += dt;
		if (icicle.timeElapsed >= 5.0f) {
			icicle.childY = icicle.PosY;
			icicle.timeElapsed = 0;
		}
	}

	AEMtx33 icicleChildMtx = createTransformMtx(10.0f, 10.0f, 0, icicle.childX, icicle.childY); 
	//std::cout << "Icicle Child Position: " << icicle.childX << " " << icicle.childY << '\n';
	AEGfxSetTransform(icicleChildMtx.m);
	AEGfxMeshDraw(icicleMesh, AE_GFX_MDM_TRIANGLES);
}

bool icicleCollision(Player &player, Icicle &icicle) {
	if (AreCirclesIntersecting(player.posX, player.posY, player.width / 2, icicle.childX, icicle.childY, 15)) {
		player.lockMovement = true; //Lock player movement
		icicle.childY = icicle.PosY; //Reset child icicle to the top
		icicle.timeElapsed = 0; //Reset time elapsed for icicle child
		icicle.cooldownElapsed = 0; //Reset cooldown elapsed for icicle drop
		return true;
	}
	return false;
}

// player default WSAD controls
//void UpdatePlayerMovement(Player *player , AEGfxVertexList* player_mesh) {
//	player->speed = AEFrameRateControllerGetFrameTime() * 300.f; //speed of player according to frame rate
//	//player movement 
//	if (AEInputCheckCurr(AEVK_W)) player->posY -= player->speed;
//	if (AEInputCheckCurr(AEVK_S)) player->posY += player->speed;
//	if (AEInputCheckCurr(AEVK_A)) player->posX += player->speed;
//	if (AEInputCheckCurr(AEVK_D)) player->posX -= player->speed;
//}

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
