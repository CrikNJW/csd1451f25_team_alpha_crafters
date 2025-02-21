// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include <iostream>
#include <vector>
#include "AEEngine.h"
#include "utils.hpp"
#include "Structs.hpp"
#include "FloatingEnemy.hpp"
#include "LCS.hpp"

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
	AEGfxVertexList* squareMesh = createSquareMesh();
	AEMtx33 playerMtx = createTransformMtx(50.0f, 50.0f, 0, 0, 0);
	f32 dt;

	//LCS Variables
	AEGfxVertexList* LCS_SelectedMesh = squareMesh; //For LCS
	bool LCS_Mode = false; //To trigger AI logic
	int LCS_CurrKey = 0;
	int LCS_ObjID = 1;
	std::vector<Floatie*> LCS_FloatingEnemies; //All gameobjects placed in the level

	//Dummy icicle array that stores coordinates of each icicle.
	Icicle* icicle = new Icicle[2]{ {-400,200}, {-300,200} };
	for (int i = 0; i < 2; i++) {
		initIcicle(icicle[i]);
	}

	//Spotlight effect Mesh
	//AEGfxVertexList* spotlightMesh = createCircleMesh();

	// Changing the window title
	AESysSetWindowTitle("Thalassa");

	// reset the system modules
	AESysReset();

	//Initialisation of Player Variables 
	// Pos X, Pox Y, Width, Height, Rotation degree, Speed, Health, maxhealth
	Player diver = {
		0.f, // Pos X
		0.f, // Pos Y
		50.f, // Width 
		50.f, // Height
		0.f, // Rotation degree
		0.f, // Speed
		5, // Health
		5, // Max health
		false, // dash flag
		1000.0f, // dash speed
		0.2f, // dash duration
		5.0f, // dash cooldown
		0.0f, // current dash time
		0.0f // dash cooldown time
	};

	//Initialisation of Boundary Variables 
	Boundaries testWall = {
	0.0f,   // X position (center) 
	-425.0f, // Y position (bottom of screen) 
	1600.0f, // Width 
	50.0f    // Height 
	};
	Boundaries testWall2{
	0.0f,   // X position (center) 
	425.0f, // Y position (top of screen) 
	1600.0f, // Width 
	50.0f    // Height 
	};
	Boundaries testWall3{
	-800.0f,   // X position (center) 
	0.0f, // Y position (top of screen) 
	50.0f, // Width 
	900.0f    // Height 
	};
	InitializeBoundary(testWall3);
	Boundaries testWall4{
	800.0f,   // X position (center) 
	0.0f, // Y position 
	50.0f, // Width 
	900.0f    // Height 

	};
	InitializeBoundary(testWall4);
	//Initialization of test ground_enemy 
	Ground_enemy ground_enemy1{
	 -200.0f,
	 200.0f,
	 100.0f,
	 100.0f,
	 0
	};

	LavaSpout volcano = { -200.0f, -180.0f }; // Volcano at (500,150)
	Boundaries volcanoPlatform = { -200.0f, -200.0f, 200.0f, 20.0f }; // platform under the volcano
	InitializeBoundary(volcanoPlatform);

	
	Boundaries platform = {400.0f, 0.0f, 500.0f, 70.0f};
	InitializeBoundary(platform);
	Ground_enemy enemy = {platform.PosX - (platform.Width / 2) + (50.0f / 2),  // Start at platform left edge
	platform.PosY + (platform.Height / 2) + (50.0f / 2), // Place on top of the platform
	50.0f, 50.0f, 0.0f, 100.0f, Ground_enemy::MOVE_RIGHT};

	// Burrowing Enemy Initialization
	Boundaries burrowingBoundary = {
	-500.0f, 0.0f,   // PosX, PosY
	50.0f, 150.0f     // Width, Height
	};

	Burrowing_enemy burrowingEnemy1 = {
	-400.0f, 200.0f,   // PosX, PosY
	40.0f, 40.0f,     // Width, Height
	10.0f,           // Speed
	150.0f,           // Detection Radius
	0.0f,             // Attack Cooldown
	false,            // isVisible
	0.0f,             // alertTimer
	false,            // spawningParticles
	Burrowing_enemy::IDLE,  // Initial state
	&burrowingBoundary //  Assign the boundary
	};

	burrowingEnemy1.PosY = burrowingBoundary.PosY;
	burrowingEnemy1.PosX = burrowingBoundary.PosX;

	// Create an enemy mesh for rendering
	AEGfxVertexList* burrowingEnemyMesh = createSquareMesh();
	AEGfxVertexList* floatieMesh = createSquareMesh();

	// Create array of boundaries 
	Boundaries boundaries_array[] = { testWall,  testWall2, burrowingBoundary, volcanoPlatform, platform, testWall4 , testWall3 };
	// boundary count to calculate amount of boundaries we need to check collision for 
	int boundaryCount = sizeof(boundaries_array) / sizeof(Boundaries); 

	// Create array of ground enemys 
	Ground_enemy* ground_enemy_array[] = { &ground_enemy1, &enemy };
	int ground_enemy_count = sizeof(ground_enemy_array) / sizeof(Ground_enemy*);

	// Create array of burrowing enemys
	Burrowing_enemy* burrowing_enemy_array[] = { &burrowingEnemy1 };
	int burrowing_enemy_count = sizeof(burrowing_enemy_array) / sizeof(Burrowing_enemy*);

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		dt = f32(AEFrameRateControllerGetFrameTime());
		// Tell the Alpha Engine to get ready to draw something.
		AEGfxSetRenderMode(AE_GFX_RM_COLOR); // Draw with Color (AE_GFX_RM_TEXTURE)
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f); // Tell the Alpha Engine to set the background to black.
		AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);

		//Trigger Level Creation System
		if (AEInputCheckTriggered(AEVK_L)) {
			LCS_Mode = !LCS_Mode;
		}

		//Level Creation System Testing
		if (LCS_Mode) {
			if (AEInputCheckTriggered(AEVK_1)) {
				LCS_SelectedMesh = squareMesh;
				LCS_ObjID = 1;
				LCS_CurrKey = 1;
			}
			else if (AEInputCheckTriggered(AEVK_2)) {
				LCS_SelectedMesh = floatieMesh;
				LCS_ObjID = 2;
				LCS_CurrKey = 2;
			}
			//-------------------------------------------------------------------------------------
			if (LCS_CurrKey == 1) { //Square Block
				//GridCoordinate clickPos = handle_LMouseClickInEditor(diver, 50, LCS_ObjID, //Insert LCS Vector Here//, LCS_SelectedMesh);
			}
			else if (LCS_CurrKey == 2) { //Floating Enemy
				//Detects click position, and adds a obj to the specified vector
				GridCoordinate clickPos = handle_LMouseClickInEditor(diver, 50, LCS_ObjID, LCS_FloatingEnemies, LCS_SelectedMesh);
			}

			//Render the object ONLY, does not trigger logic. This is because we want to see it in the LCS without it moving around.
			RenderFloatingEnemies(LCS_FloatingEnemies, diver, dt);
		}

		if (!LCS_Mode) {
			RenderFloatingEnemies(LCS_FloatingEnemies, diver, dt);
			UpdateFloatingEnemies(LCS_FloatingEnemies, diver, dt); //Loop through placed objects and draw them
		}

		//GROUND CIRCLING ENEMY SYSTEM
		// Update enemy transformation
		UpdateGroundEnemy(enemy, platform, dt);
		// Render enemy
		RenderGroundEnemy(enemy, squareMesh);

		UpdateBurrowingEnemy(burrowingEnemy1, diver.posX, diver.posY, dt);
		RenderBurrowingEnemy(burrowingEnemy1, burrowingEnemyMesh);
		RenderBoundary(burrowingBoundary, squareMesh);
		RenderBoundary(testWall4, squareMesh);
		RenderBoundary(testWall3, squareMesh);
		RenderBoundary(platform, squareMesh);
		RenderBoundary(volcanoPlatform, squareMesh);

		//CAMERA SYSTEM, PLAYER RENDERING
		AEGfxSetCamPosition(diver.posX, diver.posY); //Camera follows the player  
		// Tell the Alpha Engine to get ready to draw something.  

		//Dummy Mesh/Object to test camera movement
		UpdatePlayerPos(&diver, squareMesh, dt);

		//Player Dash ability
		PlayerDash(&diver, squareMesh, dt);

		// Render health bar
		RenderHealthBar(diver, squareMesh);

		//ICICLE RENDERING
		//Loop through icicle array and draw each icicle
		for (int i = 0; i < 2; i++) {
			icicleCollision(diver, icicle[i]);
			DrawIcicle(icicle[i], squareMesh);
			Draw_UpdateIcicleDrop(icicle[i], squareMesh, dt);
			CheckCollision(diver, icicle[i].boundaries);
		}

		for (int i = 0; i < 2; i++) {	
			lavaCollision(diver, volcano);
			Draw_UpdateLavaDrop(volcano, squareMesh, dt);
		}



		//Dummy Mesh/Object to test camera movement
		AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
		// Draw test wall
		AEMtx33 wallMtx = createTransformMtx(testWall.Width, testWall.Height, 0, testWall.PosX, testWall.PosY);
		AEGfxSetTransform(wallMtx.m);
		AEGfxMeshDraw(squareMesh, AE_GFX_MDM_TRIANGLES);
		// Draw test wall 2
		AEMtx33 wall2Mtx = createTransformMtx(testWall2.Width, testWall2.Height, 0, testWall2.PosX, testWall2.PosY);
		AEGfxSetTransform(wall2Mtx.m);
		AEGfxMeshDraw(squareMesh, AE_GFX_MDM_TRIANGLES);
		// draw test ground enemy 1  
		AEMtx33 enemy1Mtx = createTransformMtx(ground_enemy1.Width, ground_enemy1.Height, 0, ground_enemy1.PosX, ground_enemy1.PosY);
		AEGfxSetTransform(enemy1Mtx.m);  AEGfxMeshDraw(squareMesh, AE_GFX_MDM_TRIANGLES);
		// Check collisions with all boundaries
		for (int i = 0; i < boundaryCount; ++i) {
			CheckCollision(diver, boundaries_array[i]);
		}
		//collision for all ground enemy  
		for (int i = 0; i < ground_enemy_count; ++i) {
			ElasticEnemyCollision(diver, ground_enemy_array[i]->PosX, ground_enemy_array[i]->PosY, ground_enemy_array[i]->Width, ground_enemy_array[i]->Height);
		}
		//collision for all burrowing enemy
		for (int i = 0; i < burrowing_enemy_count; ++i) {
			ElasticEnemyCollision(diver, burrowing_enemy_array[i]->PosX, burrowing_enemy_array[i]->PosY, burrowing_enemy_array[i]->Width, burrowing_enemy_array[i]->Height);
		}

		//Reset all colours to 0
		AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);

		DrawBlackOverlay(squareMesh, diver, volcano);
		//SpotLight(&diver, spotlightMesh);

		// Basic way to trigger exiting the application when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist()) {
			gGameRunning = 0;
		}
		// Informing the system about the loop's end  
		AESysFrameEnd();
	}
	
	// free the system
	AEGfxMeshFree(squareMesh);
	AEGfxMeshFree(floatieMesh);
	//AEGfxMeshFree(spotlightMesh);
	AEGfxMeshFree(burrowingEnemyMesh);

	//Free the icicle array
	delete[] icicle;

	//Loop through the gameobjects vector and delete each object
	for (int i = 0; i < LCS_FloatingEnemies.size(); i++) {
		delete LCS_FloatingEnemies[i];
	}

	// free the system
	AESysExit();
}