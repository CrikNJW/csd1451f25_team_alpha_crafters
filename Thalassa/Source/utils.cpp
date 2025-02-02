#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "Structs.hpp"
#include <cmath>
#include <vector>

//Marcos for the trigo functions that take input in degree 
//cuz alpha engine only takes input in radians for function sin, cos, tan
//#define AESinDeg(x) AESin(AEDegToRad(x));
//#define AECosDeg(x) AESin(AEDegToRad(x));

/************************************
*									*
*	LEVEL CREATION SYSTEM			*
*									*
*************************************/

/*
* Creates a vector of grid coordinates for the level.
* @param squareGridLength: The length of each square grid.
* @return: A vector of grid coordinates for the level.
*/
std::vector<GridCoordinate> initializeGridSystem(s32 squareGridLength) {
	//Create a vector to store the grid coordinates
	std::vector<GridCoordinate> lineGridCoordinates;

	s32 currLength = 0;
	s32 currHeight = squareGridLength;
	s32 middleY = squareGridLength / 2;
	/*Get the coordinate of each grid, this loop basically
	stores a horizontal line of grid coordinates in the vector
	We will multiply/add to the Y coordinate for all elements
	to get a bigger area.*/
	while (currLength < 4800){
		s32 prevLength = currLength;

		currLength += squareGridLength;

		//Get the middle of the grid
		s32 middleX = (currLength + prevLength) / 2;

		//Store the grid coordinate in the vector
		lineGridCoordinates.push_back({ middleX, middleY });
	}

	// Create a vector to store the full grid coordinates
	std::vector<GridCoordinate> fullGridCoordinates;

	// Number of rows and columns
	int numRows = 50; // Number of horizontal grid lines
	int numCols = (int)lineGridCoordinates.size();

	// Generate the full grid
	for (int row = -numRows; row < numRows; ++row) {
		for (int col = 0; col < numCols; ++col) {
			GridCoordinate coord = lineGridCoordinates[col];
			coord.y += row * squareGridLength;
			fullGridCoordinates.push_back(coord);
		}
	}

	/*// Print the grid coordinates
	for (GridCoordinate coord : fullGridCoordinates) {
		std::cout << "X: " << coord.x << " Y: " << coord.y << '\n';
	}*/

	return fullGridCoordinates;
}

/*
* Gets the closest grid coordinate in std::vector grid to the mouse position
* @param grid: The vector of grid coordinates to check
* @param mouseX: The x position of the mouse
* @param mouseY: The y position of the mouse
* @return: The closest grid coordinate to the mouse
*/
GridCoordinate getClosestGridCoordinate(const std::vector<GridCoordinate>& grid, s32 mouseX, s32 mouseY, s32 playerX, s32 playerY) {
	//mouseX and mouseY currently SCREEN SPACE, we need to convert them to WORLD SPACE to account for player position.
	//We will do this by adding the player's position to the mouse position
	s32 adjustedMouseX = mouseX + playerX;
	s32 adjustedMouseY = mouseY + playerY;
	
	// Initialize the closest coordinate to the first grid, this is just a starting point so it doesn't matter
	GridCoordinate closestCoord = grid[0];

	// Use the novel pythagorean theorem to get the distance between the closest coordinate and the mouse, this is just a starting point so it doesn't matter
	double closestDist = sqrt((closestCoord.x - adjustedMouseX) * (closestCoord.x - adjustedMouseX) + (closestCoord.y - adjustedMouseY) * (closestCoord.y - adjustedMouseY));

	// For each grid coordinate, check if it is closer to the mouse than the current closest coordinate
	for (GridCoordinate coord : grid) {
		double dist = sqrt((coord.x - adjustedMouseX) * (coord.x - adjustedMouseX) + (coord.y - adjustedMouseY) * (coord.y - adjustedMouseY));
		
		// If the distance is smaller, update the closest coordinate and distance
		if (dist < closestDist) {
			closestDist = dist; // Update the closest distance
			closestCoord = coord; // Update the closest coordinate
		}
	}

	return closestCoord;
}

GridCoordinate handle_LMouseClickInEditor(const std::vector<GridCoordinate>& grid, Player& diver) {
	if (AEInputCheckReleased(AEVK_LBUTTON)) {
		s32 mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);

		//Get the closest grid coordinate to the mouse
		GridCoordinate closestCoord = getClosestGridCoordinate(grid, mouseX, mouseY, (s32)diver.posX, (s32)diver.posY);

		//Debugging
		std::cout << "Closest Coordinate " << closestCoord.x << " " << closestCoord.y << '\n';

		//Return the closest grid coordinate
		return closestCoord;
	}

	return { 0,0 };

}
/************************************
*									*
*	END OF LEVEL CREATION SYSTEM	*
*									*
*************************************/

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
	AEMtx33Scale(&scaleMtx, scaleX, scaleY); // Create a rotation matrix that rotates by 90 degrees
	// Note that PI in radians is 180 degrees. // Since 90 degrees is 180/2, 90 degrees in radians is PI/2
	AEMtx33 rotateMtx = { 0 }; 
	AEMtx33Rot(&rotateMtx, rotate_rad);
	// Create a translation matrix that translates by // 200 in the x-axis and 100 in the y-axis
	AEMtx33 translateMtx = { 0 }; 
	AEMtx33Trans(&translateMtx, translX, translY);
	// Concatenate the matrices into the 'transform' variable. // We concatenate in the order of translation * rotation * scale
	// i.e. this means we scale, then rotate, then translate. 
	AEMtx33 transformMtx = { 0 };
	AEMtx33Concat(&transformMtx, &rotateMtx, &scaleMtx); AEMtx33Concat(&transformMtx, &translateMtx, &transformMtx);
	return transformMtx;
}

int IsCircleClicked(float circle_center_x, float circle_center_y, float radius, float click_x, float click_y)
{
	// distance = sgrt of [(X1 - X2)^2 + (Y1 - Y2)^2], we eliminate the sqrt by squaring the radius later
	float distance_squared = (click_x - circle_center_x) * (click_x - circle_center_x) + (click_y - circle_center_y) * (click_y - circle_center_y);

	if (distance_squared <= radius * radius) {
		return 1;
	}
	else {
		return 0;
	}
}

int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2) {
	float collide_dist_sqr = (c1_x - c2_x) * (c1_x - c2_x) + (c1_y - c2_y) * (c1_y - c2_y);
	if (collide_dist_sqr <= (r1 + r2) * (r1 + r2)) {
		return 1;
	}
	else {
		return 0;
	}
}


void DrawBlackOverlay(AEGfxVertexList* square_mesh, Player& player) {
	f32 rec_width = f32(AEGfxGetWindowWidth());
	f32 rec_height =f32(AEGfxGetWindowHeight());
	f32 square_size = 20.f; //size of each square grid
	f32 radius = 200.f; //radius of the spotlight
	f32 buffer = 100.f; //to accomodate the rendering of squares at the side windows

	//Dim the black colour rectangle
	//AEGfxSetBlendMode(AE_GFX_BM_MULTIPLY); //change to AE_GFX_BM_MULTIPLY for complete darkness
	//Adjust the opacity of the darkness
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.95f);
	
	f32 x_pos = player.posX - (rec_width/2.0f) - buffer;
	f32 y_pos = player.posY - (rec_height/2.0f) - buffer;
	for (int y = 0; y_pos + (y* square_size) <= player.posY + rec_height/2.0f + buffer; y++){
		for (int x = 0; x_pos + (x * square_size) <= player.posX + rec_width/2.0f + buffer; x++) {
			f32 x_coord = x_pos + (x * square_size);
			f32 y_coord = y_pos + (y * square_size);
			//Only draw the squares if it is not inside the circle
			if (!IsCircleClicked(player.posX, player.posY, radius, x_coord, y_coord)) {
				AEMtx33 black_overlayMtx = createTransformMtx(square_size, square_size, 0, x_coord, y_coord);
				AEGfxSetTransform(black_overlayMtx.m);
				AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}
	AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);//clear the colours
}

void SpotLight(Player* player, AEGfxVertexList* circle_mesh) {
	f32 radius = 200.f;
	AEMtx33 spotlightMtx = createTransformMtx(radius, radius, 0, player->posX, player->posY);
	AEGfxSetBlendMode(AE_GFX_BM_ADD);
	AEGfxSetColorToAdd(2.0f, 2.0f, 2.0f, 0.5f);
	
	AEGfxSetTransform(spotlightMtx.m);
	AEGfxMeshDraw(circle_mesh, AE_GFX_MDM_TRIANGLES);
	//AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
	
}
//Hi Varick

void UpdatePlayerPos(Player *player, AEGfxVertexList* player_mesh, f32 dt) {
	if (player->lockMovement == true) {
		player->lockTimeElapsed += dt;
		if (player->lockTimeElapsed >= player->lockTime) {
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
	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f); // PLayer Colour (white)
	AEMtx33 playerMtx = createTransformMtx(player->width, player->height, AEDegToRad(player->rotate_angle), player->posX, player->posY);
	AEGfxSetTransform(playerMtx.m); 
	AEGfxMeshDraw(player_mesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
}

void PlayerDash(Player* player, AEGfxVertexList* CooldownMesh, f32 dt) { // player dash movement
	// Don't allow dashing if movement is locked
	if (player->lockMovement) {
		return;
	}
	// Constants for the cooldown bar appearance
	const float BAR_WIDTH = 100.0f;
	const float BAR_HEIGHT = 10.0f;
	const float OFFSET_Y = 50.0f;  // Distance above player

	// Update dash cooldown
	if (player->dashCooldownTimer > 0) {
		player->dashCooldownTimer -= dt;
		// Draw cooldown background (grey bar)
		AEMtx33 backgroundMtx = createTransformMtx(BAR_WIDTH, BAR_HEIGHT, 0.0f,
			player->posX,
			player->posY + OFFSET_Y);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(0.5f, 0.5f, 0.5f, 1.0f);  // Grey color
		AEGfxSetTransform(backgroundMtx.m);
		AEGfxMeshDraw(CooldownMesh, AE_GFX_MDM_TRIANGLES);

		// Draw cooldown fill (blue bar)
		float fillPercentage = (player->dashCooldownTimer / player->dashCooldown);
		AEMtx33 fillMtx = createTransformMtx(BAR_WIDTH * fillPercentage, BAR_HEIGHT, 0.0f,
			player->posX - (BAR_WIDTH * (1 - fillPercentage)) / 2,
			player->posY + OFFSET_Y);
		AEGfxSetColorToAdd(0.0f, 0.0f, 1.0f, 1.0f);  // Blue color
		AEGfxSetTransform(fillMtx.m);
		AEGfxMeshDraw(CooldownMesh, AE_GFX_MDM_TRIANGLES);
	}

	// Check for dash input (SPACE key) and if we can dash
	if (AEInputCheckTriggered(AEVK_SPACE) && player->dashCooldownTimer <= 0 && !player->isDashing) {
		player->isDashing = true;
		player->currentDashTime = player->dashDuration;
		player->dashCooldownTimer = player->dashCooldown;
	}

	// Handle the actual dash movement
	if (player->isDashing) {
		// Move player in direction they're facing
		float dashDistance = player->dashSpeed * dt;
		player->posX += dashDistance * AECos(AEDegToRad(player->rotate_angle));
		player->posY += dashDistance * AESin(AEDegToRad(player->rotate_angle));

		//add animation here >;

		// Update dash duration
		player->currentDashTime -= dt;
		if (player->currentDashTime <= 0) {
			player->isDashing = false;
		}

		
	}
}

void initIcicle(Icicle& icicle) {
	icicle.childX = icicle.PosX;
	icicle.childY = icicle.PosY;
	icicle.boundaries.PosX = icicle.PosX;
	icicle.boundaries.PosY = icicle.PosY;
}

//Draw icicle at the given position
void DrawIcicle(Icicle &icicle , AEGfxVertexList* icicleMesh) {
	AEGfxSetColorToAdd(0.0f, 1.0f, 1.0f, 1.0f); // Icicle Colour (blue)
	AEMtx33 icicleMtx = createTransformMtx(icicle.boundaries.Width, icicle.boundaries.Height, 0, icicle.PosX, icicle.PosY);
	AEGfxSetTransform(icicleMtx.m);
	AEGfxMeshDraw(icicleMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
}

//Draw icicle child and make it repeatedly drop icicles.
void Draw_UpdateIcicleDrop(Icicle &icicle, AEGfxVertexList* icicleMesh, f32 dt) {
	AEGfxSetColorToAdd(0.0f, 1.0f, 1.0f, 1.0f); // Icicle Drop Colour (blue)
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
	AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
}

bool icicleCollision(Player &player, Icicle &icicle) {
	if (AreCirclesIntersecting(player.posX, player.posY, player.width / 2, icicle.childX, icicle.childY, 15)) {
		if (!player.lockMovement) { // Only take damage if not locked to imitate "invincibility"
			player.takedamage(1); // Reduce health by 1 (can change)
			player.lockMovement = true; // Lock movement after damage
		}
		icicle.childY = icicle.PosY; //Reset child icicle to the top
		icicle.timeElapsed = 0; //Reset time elapsed for icicle child
		icicle.cooldownElapsed = 0; //Reset cooldown elapsed for icicle drop
		return true;
	}
	return false;
}

// player default WSAD controls
void UpdatePlayerMovement(Player *player , AEGfxVertexList* player_mesh) {
	player->speed = f32(AEFrameRateControllerGetFrameTime()) * 300.f; //speed of player according to frame rate //player movement 
	if (AEInputCheckCurr(AEVK_W)) player->posY -= player->speed; 
	if (AEInputCheckCurr(AEVK_S)) player->posY += player->speed;
	if (AEInputCheckCurr(AEVK_A)) player->posX += player->speed; 
	if (AEInputCheckCurr(AEVK_D)) player->posX -= player->speed;
 }

 //collision for player and boundary 
 void CheckCollision(Player& player, const Boundaries& boundary) {
	 // Calculate bounds
	 float playerLeft = player.posX - player.width / 2; float playerRight = player.posX + player.width / 2;
	 float playerBottom = player.posY - player.height / 2; float playerTop = player.posY + player.height / 2;
	 float boundaryLeft = boundary.PosX - boundary.Width / 2;
	 float boundaryRight = boundary.PosX + boundary.Width / 2; float boundaryBottom = boundary.PosY - boundary.Height / 2;
	 float boundaryTop = boundary.PosY + boundary.Height / 2;
	 // Check if there's a collision 
	if (playerLeft < boundaryRight && playerRight > boundaryLeft && playerBottom < boundaryTop&& playerTop > boundaryBottom) {
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
		 } else if (minOverlap == overlapRight) {
			player.posX -= overlapRight;
		 }
		 else if (minOverlap == overlapTop) {
			 player.posY -= overlapTop;
		 } else if (minOverlap == overlapBottom) {
		  player.posY += overlapBottom;
		 }
	}
}



 // for bounce collision between player and enemy
 void ElasticEnemyCollision(Player& player, f32 enemy_x, f32 enemy_y, f32 enemy_width, f32 enemy_height) {
// need a flag for checking if player dashing D:
	 if (!player.isDashing) {
		 //flag for collision
		 bool isBouncing = false;
		 // Calculate the edges of the centered enemy rectangle 
		 float enemyLeft = enemy_x - enemy_width / 2;
		 float enemyRight = enemy_x + enemy_width / 2;
		 float enemyTop = enemy_y - enemy_height / 2;
		 float enemyBottom = enemy_y + enemy_height / 2;
		 // Calculate the edges of the player rectangle  
		 float playerLeft = player.posX - player.width / 2;
		 float playerRight = player.posX + player.width / 2;
		 float playerTop = player.posY - player.height / 2;
		 float playerBottom = player.posY + player.height / 2;
		 // Check for collision 
		 if (playerRight > enemyLeft && playerLeft < enemyRight && playerBottom > enemyTop && playerTop < enemyBottom) {
			 isBouncing = true;
			 // Bounce back distance 
			 float bounceBackDistance = 75.0f; // Adjust this value to control bounce intensity
			 float moveDirectionMultiplier = 0.0f;

			 // Check movement input states
			 bool movingForward = AEInputCheckCurr(AEVK_W);
			 bool movingBackward = AEInputCheckCurr(AEVK_S);

			 if (movingForward && movingBackward) {
				 // Conflict: Player presses both W and S
				 moveDirectionMultiplier = 1.0f; // update player pos always priotises w 
				 std::cout << "movement conflict detected: W and S pressed together\n";
			 }
			 else if (movingBackward) {
				 moveDirectionMultiplier = -1.0f;
				 std::cout << "movement currently: backwards\n";
			 }
			 else if (movingForward) {
				 moveDirectionMultiplier = 1.0f;
				 std::cout << "movement currently: forwards\n";
			 }

			 // Apply bounce effect 
			 if (isBouncing) {
				 bounceBackDistance -= 25.0f;
				 player.posX -= moveDirectionMultiplier * bounceBackDistance * AECos(AEDegToRad(player.rotate_angle));
				 player.posY -= moveDirectionMultiplier * bounceBackDistance * AESin(AEDegToRad(player.rotate_angle));
				 if (bounceBackDistance <= 0) isBouncing = false;
			 }
			 
			 player.health -= 1;
			 // Bounce the player back in the opposite direction of current movement  
			
		 }
	 }
	 else {
		 // During dash, check for collision but don't bounce
		  // Calculate the edges of the centered enemy rectangle 
		 float enemyLeft = enemy_x - enemy_width / 2;
		 float enemyRight = enemy_x + enemy_width / 2;
		 float enemyTop = enemy_y - enemy_height / 2;
		 float enemyBottom = enemy_y + enemy_height / 2;
		 // Calculate the edges of the player rectangle  
		 float playerLeft = player.posX - player.width / 2;
		 float playerRight = player.posX + player.width / 2;
		 float playerTop = player.posY - player.height / 2;
		 float playerBottom = player.posY + player.height / 2;

		 // Check for collision during dash
		 if (playerRight > enemyLeft && playerLeft < enemyRight &&
			 playerBottom > enemyTop && playerTop < enemyBottom) {
			 
			 //add enemy taken damage
			 //maybe add animation to signify enemy was killed
		 }
	 }
	 
}


void InitializeBoundary(Boundaries& platform) {

	AEMtx33 scaleMtx, translateMtx;
	AEMtx33Scale(&scaleMtx, platform.Width, platform.Height);
	AEMtx33Trans(&translateMtx, platform.PosX, platform.PosY);
	AEMtx33Concat(&platform.finalTransform, &translateMtx, &scaleMtx);
}



void UpdateGroundEnemy(Ground_enemy& enemy, Boundaries &boundary, float dt) {
	
	float platformLeft = boundary.PosX - (boundary.Width / 2) - (enemy.Width / 2);
	float platformRight = boundary.PosX + (boundary.Width / 2) + (enemy.Width / 2);
	float platformTop = boundary.PosY + (boundary.Height / 2) + (enemy.Width / 2);
	float platformBottom = boundary.PosY - (boundary.Height / 2) - (enemy.Width / 2);

	switch (enemy.MovementState) {
	case Ground_enemy::MOVE_RIGHT:
		enemy.PosX += enemy.speed * dt;
		if (enemy.PosX >= platformRight) {
			enemy.PosX = platformRight;  // Stop at edge
			enemy.MovementState = Ground_enemy::MOVE_DOWN;     // Change direction
		}
		break;

	case Ground_enemy::MOVE_DOWN:
		enemy.PosY -= enemy.speed * dt;
		if (enemy.PosY <= platformBottom) {
			enemy.PosY = platformBottom;
			enemy.MovementState = Ground_enemy::MOVE_LEFT;
		}
		break;

	case Ground_enemy::MOVE_LEFT:
		enemy.PosX -= enemy.speed * dt;
		if (enemy.PosX <= platformLeft) {
			enemy.PosX = platformLeft;
			enemy.MovementState = Ground_enemy::MOVE_UP;
		}
		break;

	case Ground_enemy::MOVE_UP:
		enemy.PosY += enemy.speed * dt;
		if (enemy.PosY >= platformTop) {
			enemy.PosY = platformTop;
			enemy.MovementState = Ground_enemy::MOVE_RIGHT;
		}
		break;
	}

	AEMtx33 scaleMtx, translateMtx;
	AEMtx33Scale(&scaleMtx, enemy.Width, enemy.Height);
	AEMtx33Trans(&translateMtx, enemy.PosX, enemy.PosY);
	AEMtx33Concat(&enemy.finalTransform, &translateMtx, &scaleMtx);
}
	
void RenderGroundEnemy(Ground_enemy& enemy, AEGfxVertexList* mesh) {
	// Set enemy color
	AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f); // Red color

	// Apply transformation matrix
	AEGfxSetTransform(enemy.finalTransform.m);

	// Draw enemy
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
}

//health squares
void RenderHealthBar(const Player& player, AEGfxVertexList* mesh) {
	float squareSize = 20.0f;  // Size of each health square
	float spacing = 5.0f;      // Spacing between squares
	float totalWidth = (squareSize + spacing) * player.health - spacing;  // Total width for current health squares
	float barPosX = player.posX - totalWidth / 2.0f;  // Start position (centered based on current health)
	float barPosY = player.posY + player.height + 30.0f;  // Position above the player

	// Render one square for each point of current health
	for (int i = 0; i < player.health; ++i) {
		float squarePosX = barPosX + i * (squareSize + spacing);

		// Set color for current health (green)
		AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f);

		// Create and render the square
		AEMtx33 squareTransform = createTransformMtx(squareSize, squareSize, 0.0f, squarePosX, barPosY);
		AEGfxSetTransform(squareTransform.m);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
	}
	AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
}

void Draw_UpdateLavaDrop(LavaSpout& lavaSpout, AEGfxVertexList* lavaMesh, float dt) {
	AEGfxSetColorToAdd(1.0f, 0.61f, 0.0f, 1.0f); // Red for lava

	// Handle cooldown before respawn
	if (!lavaSpout.isActive) {
		if (lavaSpout.cooldownElapsed < lavaSpout.cooldown) {
			lavaSpout.cooldownElapsed += dt;
			return;  // Exit function while waiting for cooldown
		}
		else {
			// Respawn lava at volcano position with new random direction
			lavaSpout.lavaX = lavaSpout.PosX;
			lavaSpout.lavaY = lavaSpout.PosY;

			// Random horizontal velocity between -30 to -60 or 30 to 60
			lavaSpout.velocityX = (f32)((rand() % 31 + 30) * (rand() % 2 == 0 ? 1 : -1));
			lavaSpout.velocityY = 120.0f;  // Initial upward velocity

			lavaSpout.timeElapsed = 0;
			lavaSpout.cooldownElapsed = 0.0f;
			lavaSpout.isActive = true;
		}
	}

	// Apply movement to lava (parabolic arc)
	lavaSpout.lavaX += lavaSpout.velocityX * dt;
	lavaSpout.lavaY += lavaSpout.velocityY * dt;

	// Apply gravity effect
	lavaSpout.velocityY -= 200.0f * dt;  // Gravity pulls lava down

	lavaSpout.timeElapsed += dt;

	// Reset lava if it falls too low
	if (lavaSpout.lavaY <= lavaSpout.PosY - 300.0f) {
		lavaSpout.isActive = false;  // Hide lava and start cooldown
	}

	// Render the lava drop only if active
	if (lavaSpout.isActive) {
		AEMtx33 lavaTransform = createTransformMtx(15.0f, 15.0f, 0, lavaSpout.lavaX, lavaSpout.lavaY);
		AEGfxSetTransform(lavaTransform.m);
		AEGfxMeshDraw(lavaMesh, AE_GFX_MDM_TRIANGLES);
	}

	AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
}

bool lavaCollision(Player& player, LavaSpout& lavaSpout) {
	if (lavaSpout.isActive && AreCirclesIntersecting(player.posX, player.posY, player.width / 2, lavaSpout.lavaX, lavaSpout.lavaY, 15)) {
		player.takedamage(1); // Reduce health by 1

		// Hide the lava drop but keep the cooldown running
		lavaSpout.isActive = false;
		lavaSpout.cooldownElapsed = 0.0f; // Start cooldown timer

		return true; // Collision happened
	}
	return false;
}

void UpdateBurrowingEnemy(Burrowing_enemy& enemy, float playerX, float playerY, AEGfxVertexList* lavaMesh, float dt) {
	float dx = playerX - enemy.PosX;
	float dy = playerY - enemy.PosY;
	float distanceSquared = dx * dx + dy * dy;

	float attackOffset = 60.0f;  // How far the enemy should pop out
	float originalOffset = 0.0f; // Default position (inside boundary)

	switch (enemy.State) {
		case Burrowing_enemy::IDLE:
		enemy.PosX = enemy.boundary->PosX + originalOffset; // Ensure it's inside the boundary
		if (sqrt(distanceSquared) < enemy.detectionRadius) {
			enemy.State = Burrowing_enemy::ALERT;
			enemy.alertTimer = 0.3f;  // Small delay before popping out
			enemy.isVisible = true;
		}
		break;

        case Burrowing_enemy::ALERT:
            if (enemy.alertTimer > 0) {
                enemy.alertTimer -= dt;

			} else {
				enemy.State = Burrowing_enemy::ATTACKING;
				enemy.attackCooldown = 0.3f; // Stays out for a short moment
				enemy.Width = 100.0f; // Instantly stretch out fully
				enemy.PosX = enemy.boundary->PosX - (enemy.Width / 2.0f); // Adjust position instantly
			}
			break;

		case Burrowing_enemy::ATTACKING:
			if (enemy.attackCooldown > 0) {
				enemy.attackCooldown -= dt;
				enemy.Width += 300.0f * dt;  
				if (enemy.Width > 100.0f) enemy.Width = 100.0f; // Cap width
			}
			else {
				enemy.State = Burrowing_enemy::RETREATING;
			}
			break;

		case Burrowing_enemy::RETREATING:
			enemy.Width -= 1200.0f * dt;  // Shrink back
			if (enemy.Width < 40.0f) {
				enemy.Width = 40.0f;
				enemy.PosX = enemy.boundary->PosX; // Reset position
				enemy.isVisible = false;
				enemy.State = Burrowing_enemy::WAITING;
				enemy.alertTimer = 1.5f; // Delay before popping out again
			}
			break;

		case Burrowing_enemy::WAITING:
			if (sqrt(distanceSquared) > enemy.detectionRadius) { // Player moved away
				enemy.State = Burrowing_enemy::IDLE; // Ready to pop out again
			}
			break;
	}
}

void RenderBurrowingEnemy(Burrowing_enemy& enemy, AEGfxVertexList* BurrowingEnemymesh) {
	if (!enemy.isVisible) return; // Only render if the enemy has emerged

	AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f); // Red color for the enemy

	float renderPosX = enemy.boundary->PosX - (enemy.Width / 2.0f); // Keep left-aligned
	float renderPosY = enemy.boundary->PosY;

	// Create transformation matrix for positioning
	AEMtx33 enemyTransform = createTransformMtx(enemy.Width, enemy.Height, 0.0f, renderPosX, renderPosY);
	AEGfxSetTransform(enemyTransform.m);

	// Draw enemy mesh
	AEGfxMeshDraw(BurrowingEnemymesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
}

void RenderBoundary(Boundaries& boundary, AEGfxVertexList* platformMesh) {
	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f); // Green for platform

	AEMtx33 boundaryTransform = createTransformMtx(boundary.Width, boundary.Height, 0.0f, boundary.PosX, boundary.PosY);
	AEGfxSetTransform(boundaryTransform.m);
	AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
}