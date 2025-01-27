#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "Structs.hpp"

//Marcos for the trigo functions that take input in degree 
//cuz alpha engine only takes input in radians for function sin, cos, tan
#define AESinDeg(x) AESin(AEDegToRad(x));
#define AECosDeg(x) AESin(AEDegToRad(x));

/*Creates a white square mesh, drawing from the center outwards in every direction, this is done to preserve the center coordinate*/
AEGfxVertexList* createSquareMesh() {
	u32 white = 0xFFFFFFFF;
	u32 red = 0xFFFF0000;
	
	AEGfxMeshStart();
	// Draw square mesh
	AEGfxTriAdd(
		-0.5f, -0.5f, white, 0.0f, 1.0f,  // bottom-left: white
		0.5f, -0.5f, red, 1.0f, 1.0f,   // bottom-right: white
		-0.5f, 0.5f, white, 0.0f, 0.0f);  // top-left: white

	AEGfxTriAdd(
		0.5f, -0.5f, red, 1.0f, 1.0f,   // bottom-right: white
		0.5f, 0.5f, red, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, white, 0.0f, 0.0f);  // top-left: white
	// Saving the mesh (list of triangles) in mesh
	return AEGfxMeshEnd();
	////END OF MESH
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

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	// Reason why use half width and height is because the rectangle button was drew from the middle
	float half_width = area_width / 2.0;
	float half_height = area_height / 2.0;

	// Compute the boundaries of the button rectangle
	float left_bound = area_center_x - half_width;
	float right_bound = area_center_x + half_width;
	float top_bound = area_center_y - half_height;
	float bottom_bound = area_center_y + half_height;

	// Check if the Mouse_Input is within the boundaries
	if (click_x >= left_bound && click_x <= right_bound && click_y >= top_bound && click_y <= bottom_bound) {
		return 1;
	}
	else {
		return 0;
	}
}

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	float radius = diameter / 2.0;
	// distance = sgrt of [(X1 - X2)^2 + (Y1 - Y2)^2], we eliminate the sqrt by squaring the radius later
	float distance_squared = (click_x - circle_center_x) * (click_x - circle_center_x) + (click_y - circle_center_y) * (click_y - circle_center_y);

	if (distance_squared <= radius * radius) {
		return 1;
	}
	else {
		return 0;
	}
}

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


void UpdatePlayerPos(Player *player, AEGfxVertexList* player_mesh) {
	player->speed = AEFrameRateControllerGetFrameTime() * 300.f; //speed of player according to frame rate
	f32 rotate_degree = 2.f; //rotation degree is set to 2 degree when trigerred

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

	//Draw the player Mesh
	AEGfxSetColorToMultiply(0.5f, 0.5f, 0.5f, 1.0f); // PLayer Colour (grey)
	AEMtx33 playerMtx = createTransformMtx(player->width, player->height, AEDegToRad(player->rotate_angle), player->posX, player->posY);
	AEGfxSetTransform(playerMtx.m);
	AEGfxMeshDraw(player_mesh, AE_GFX_MDM_TRIANGLES);
}

void DrawIcicle(f32 posX, f32 posY , AEGfxVertexList* icicleMesh) {
	AEGfxSetColorToMultiply(0.0f, 0.0f, 1.0f, 1.0f); // Icicle Colour (blue)
	AEMtx33 icicleMtx = createTransformMtx(30.0f, 30.0f, 0, posX, posY);
	AEGfxSetTransform(icicleMtx.m);
	AEGfxMeshDraw(icicleMesh, AE_GFX_MDM_TRIANGLES);
}

void Draw_UpdateIcicleDrop(Icicle &icicle, AEGfxVertexList* icicleMesh, f32 dt) {
	AEGfxSetColorToMultiply(0.0f, 0.0f, 1.0f, 1.0f); // Icicle Drop Colour (blue)
	icicle.childY -= icicle.dropSpeed * dt;
	icicle.timeElapsed += dt;
	if (icicle.timeElapsed >= 3.0f) {
		icicle.childY = icicle.PosY;
		icicle.timeElapsed = 0;
	}

	AEMtx33 icicleChildMtx = createTransformMtx(10.0f, 10.0f, 0, icicle.childX, icicle.childY); 
	std::cout << "Icicle Child Position: " << icicle.childX << " " << icicle.childY << '\n';
	AEGfxSetTransform(icicleChildMtx.m);
	AEGfxMeshDraw(icicleMesh, AE_GFX_MDM_TRIANGLES);
}
