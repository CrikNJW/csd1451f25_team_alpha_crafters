#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>

/*Creates a white square mesh, drawing from the center outwards in every direction, this is done to preserve the center coordinate*/
AEGfxVertexList* createSquareMesh() {
	AEGfxMeshStart();
	// Draw square mesh
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,  // bottom-left: white
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,   // bottom-right: white
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top-left: white

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,   // bottom-right: white
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top-left: white
	// Saving the mesh (list of triangles) in mesh
	return AEGfxMeshEnd();
	////END OF MESH
}

/*Creates a 3x3 transformation matrix based on the scale, rotate and translate parameters provided.*/
AEMtx33 createTransformMtx(f32 scaleX, f32 scaleY, int rotate, f32 translX, f32 translY) {
	// Create a scale matrix that scales by 500 x and y
	AEMtx33 scaleMtx = { 0 };
	AEMtx33Scale(&scaleMtx, scaleX, scaleY);
	// Create a rotation matrix that rotates by 90 degrees
	// Note that PI in radians is 180 degrees.
	// Since 90 degrees is 180/2, 90 degrees in radians is PI/2
	AEMtx33 rotateMtx = { 0 };
	AEMtx33Rot(&rotateMtx, rotate);
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

/*Takes in a reference to a AEMtx33 matrix, and multiplies the matrix based with a new transformation matrix based on player movement*/
void handlePlayerMovement(AEMtx33& playerMtx, AEVec2& playerCoord) {
	//Check player WASD input
	if (AEInputCheckCurr(AEVK_W)) {
		//std::cout << "W Pressed\n";
		AEMtx33 movementMtx = createTransformMtx(1, 1, 0, 0, 4);
		AEMtx33Concat(&playerMtx, &movementMtx, &playerMtx);
		playerCoord.y += 4;
		//playerCenter = getCenterFromCircleMtx(transform);
	}
	if (AEInputCheckCurr(AEVK_S)) {
		//std::cout << "S Pressed\n";
		AEMtx33 movementMtx = createTransformMtx(1, 1, 0, 0, -4);
		AEMtx33Concat(&playerMtx, &movementMtx, &playerMtx);
		playerCoord.y -= 4;
		//playerCenter = getCenterFromCircleMtx(transform);
	}
	if (AEInputCheckCurr(AEVK_A)) {
		//std::cout << "A Pressed\n";
		AEMtx33 movementMtx = createTransformMtx(1, 1, 0, -4, 0);
		AEMtx33Concat(&playerMtx, &movementMtx, &playerMtx);
		playerCoord.x -= 4;
		//playerCenter = getCenterFromCircleMtx(transform);
	}
	if (AEInputCheckCurr(AEVK_D)) {
		//std::cout << "D Pressed\n";
		AEMtx33 movementMtx = createTransformMtx(1, 1, 0, 4, 0);
		AEMtx33Concat(&playerMtx, &movementMtx, &playerMtx);
		playerCoord.x += 4;
		//playerCenter = getCenterFromCircleMtx(transform);
	}
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
