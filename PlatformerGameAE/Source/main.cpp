// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "utils.hpp"



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
	f32 camPosX = 0, camPosY = 0;

	AEGfxVertexList* dummyMesh = createSquareMesh();
	AEMtx33 dummyMtx = createTransformMtx(100.0f, 100.0f, 0, 200.0, 0);

	// Changing the window title
	AESysSetWindowTitle("Thalassa");

	// reset the system modules
	AESysReset();

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		// Basic way to trigger exiting the application
		// when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f); // Set the background to black.
		handlePlayerMovement(playerMtx, playerCoord); //Handle basic WASD movement
		AEGfxSetCamPosition(playerCoord.x, playerCoord.y); //Set the camera to follow the player
		AEGfxGetCamPosition(&camPosX, &camPosY);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR); //Set render mode to color instead of textures
		AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		//Draw the player
		AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 0.0f);
		AEGfxSetTransform(playerMtx.m);
		AEGfxMeshDraw(playerMesh, AE_GFX_MDM_TRIANGLES);

		//Dummy Mesh/Object to test camera movement
		AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 0.0f);
		AEGfxSetTransform(dummyMtx.m);
		AEGfxMeshDraw(dummyMesh, AE_GFX_MDM_TRIANGLES);


		//Debugging
		std::cout << "Player Location" << playerCoord.x << " " << playerCoord.y << '\n';
		std::cout << "Camera Position: " << camPosX << "," << camPosY << '\n';

		// Informing the system about the loop's end
		AESysFrameEnd();

	}

	// free the system
	AESysExit();
}