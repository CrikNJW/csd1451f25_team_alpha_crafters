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
	AEGfxVertexList *playerMesh = createSquareMesh();
	AEMtx33 playerMtx = createTransformMtx(50.0f, 50.0f, 0, 0, 0);
	AEVec2 playerCoord = { 0, 0 };


	AEGfxVertexList* dummyMesh = createSquareMesh();
	AEMtx33 dummyMtx = createTransformMtx(100.0f, 100.0f, 0, 200.0, 0);

	
	// Changing the window title
	AESysSetWindowTitle("Thalassa");

	// reset the system modules
	AESysReset();

	//Initialisation of Player Variables
	// Pos X, Pox Y, Width, Height, Rotation degree, Speed, Health
	Player diver = { 0.f, 0.f, 50.f, 50.f, 0.f, 0.f, 3 };

	//Initialisation of Boundary Variables
	Boundaries testWall = {
	0.0f,   // X position (center)
	-400.0f, // Y position (bottom of screen)
	1600.0f, // Width
	50.0f    // Height
	};

	Boundaries testWall2{
	0.0f,   // X position (center)
	425.0f, // Y position (top of screen)
	1600.0f, // Width
	50.0f    // Height

	};

	//Initialization of test ground_enemy
	Ground_enemy ground_enemy1{
		100.0f,
		100.0f,
		100.0f,
		100.0f,
		0

	};

	// Create array of boundaries
	Boundaries boundaries_array[] = { testWall,  testWall2 };
	// boundary count to calculate amount of boundaries we need to check collision for
	int boundaryCount = sizeof(boundaries_array) / sizeof(Boundaries);

	// Create array of ground enemys
	Ground_enemy Ground_enemy_array[] = { ground_enemy1 };
	int Ground_enemy_count = sizeof(Ground_enemy_array) / sizeof(Ground_enemy);

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f); // Tell the Alpha Engine to set the background to black.
		AEGfxSetCamPosition(diver.posX, diver.posY); //Camera follows the player
		
		// Tell the Alpha Engine to get ready to draw something.
		AEGfxSetRenderMode(AE_GFX_RM_COLOR); // Draw with Texture /to draw with color, use (AF_GFX_RM_COLOR)
		
		//PLAYER RENDERING
		UpdatePlayerPos(&diver, playerMesh);
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
			HandleEnemyCollision(diver, Ground_enemy_array[i]);
		}

		//Debugging
		//std::cout << "Player Location" << playerCoord.x << " " << playerCoord.y << '\n';

		// Basic way to trigger exiting the application when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		// Informing the system about the loop's end
		AESysFrameEnd();
	}
	
	//Free the Mesh
	AEGfxMeshFree(playerMesh);
	AEGfxMeshFree(dummyMesh);
	// free the system
	AESysExit();
}