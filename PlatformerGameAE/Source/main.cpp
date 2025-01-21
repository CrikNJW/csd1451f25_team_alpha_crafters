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

	//Initialize player variables
	AEGfxVertexList* playerMesh = createSquareMesh();
	AEMtx33 playerMtx = createTransformMtx(50.0f, 50.0f, 0, 0, 0);
	AEVec2 playerCoord = { 0, 0 };

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

		AEGfxSetRenderMode(AE_GFX_RM_COLOR); //Set render mode to color instead of textures
		AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		//Draw the player
		AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 0.0f);
		AEGfxSetTransform(playerMtx.m);
		AEGfxMeshDraw(playerMesh, AE_GFX_MDM_TRIANGLES);

		//std::cout << "Player Location" << playerMtx.m[0][2] << " " << playerMtx.m[1][2] << '\n';
		std::cout << "Player Location: " << playerCoord.x << "," << playerCoord.y << '\n';

		//AEGfxSetCamPosition(250, 250);

		// Informing the system about the loop's end
		AESysFrameEnd();

	}

	// free the system
	AESysExit();
}