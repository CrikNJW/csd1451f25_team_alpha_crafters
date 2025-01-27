// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "utils.hpp"
#include "Structs.hpp"

// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int gGameRunning = 1;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	//Initialize variables
	AEGfxVertexList* GroundEnemyMesh = createSquareMesh();

	AEGfxVertexList* PlatformMesh = createSquareMesh();

	// Changing the window title
	AESysSetWindowTitle("Thalassa");

	// reset the system modules
	AESysReset();

	//Initialisation of Player Variables
	// Pos X, Pox Y, Width, Height, Rotation degree, Speed, Health
	Platform platform = {400.0f, 300.0f, 500.0f, 70.0f};
	InitializePlatform(platform);
	Ground_enemy enemy = {platform.PosX - (platform.width / 2) + (50.0f / 2),  // Start at platform left edge
	platform.PosY + (platform.height / 2) + (50.0f / 2), // Place on top of the platform
	50.0f, 50.0f, 0.0f, 100.0f, MOVE_RIGHT};

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f); // Tell the Alpha Engine to set the background to black.

		float dt = AEFrameRateControllerGetFrameTime();
		
		// Tell the Alpha Engine to get ready to draw something.
		AEGfxSetRenderMode(AE_GFX_RM_COLOR); // Draw with Texture /to draw with color, use (AF_GFX_RM_COLOR)
		
		//PLAYER RENDERING
		// Update enemy transformation
		UpdateGroundEnemy(enemy, platform, dt);
		// Render enemy
		RenderGroundEnemy(enemy, GroundEnemyMesh);

		RenderPlatform(platform, PlatformMesh);
<<<<<<< HEAD
		//UpdatePlayerPos(&diver, playerMesh);
		//UpdatePlayerMovement(&diver, playerMesh);
		
		////Dummy Mesh/Object to test camera movement
		AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 0.0f);
		//AEGfxSetTransform(dummyMtx.m);
		//AEGfxMeshDraw(dummyMesh, AE_GFX_MDM_TRIANGLES);

		// Draw test wall
		AEMtx33 wallMtx = createTransformMtx(testWall.Width, testWall.Height, 0, testWall.PosX, testWall.PosY);
		AEGfxSetTransform(wallMtx.m);
		AEGfxMeshDraw(dummyMesh, AE_GFX_MDM_TRIANGLES);

		// Draw test wall 2
		AEMtx33 wall2Mtx = createTransformMtx(testWall2.Width, testWall2.Height, 0, testWall2.PosX, testWall2.PosY);
		AEGfxSetTransform(wall2Mtx.m);
		AEGfxMeshDraw(dummyMesh, AE_GFX_MDM_TRIANGLES);

		// draw test ground enemy 1
		AEMtx33 enemy1Mtx = createTransformMtx(ground_enemy1.Width, ground_enemy1.Height, 0, ground_enemy1.PosX, ground_enemy1.PosY);
		AEGfxSetTransform(enemy1Mtx.m);
		AEGfxMeshDraw(dummyMesh, AE_GFX_MDM_TRIANGLES);

		// Check collisions with all boundaries
		for (int i = 0; i < boundaryCount; ++i) {
			CheckCollision(diver, boundaries_array[i]);
		}

		//collision for all ground enemy
		for (int i = 0; i < Ground_enemy_count; ++i) {
			ElasticEnemyCollision(diver, Ground_enemy_array[i]);
		}

		//Debugging
		//std::cout << "Player Location" << playerCoord.x << " " << playerCoord.y << '\n';
=======
>>>>>>> parent of c81e7e0 (Merge branch 'main' into Trisha-GroundEnemyAI)

		// Basic way to trigger exiting the application when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		// Informing the system about the loop's end
		AESysFrameEnd();
	}
	
	//Free the Mesh
	AEGfxMeshFree(GroundEnemyMesh);
	AEGfxMeshFree(PlatformMesh);
	// free the system
	AESysExit();
}