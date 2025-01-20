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

	AEGfxVertexList* playerMesh = createSquareMesh();
	AEMtx33 playerMtx = CreateTransformMtx(50.0f, 50.0f, 0, 0, 0);

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

		//Check player WASD input
		if (AEInputCheckCurr(AEVK_W)) {
			//std::cout << "W Pressed\n";
			AEMtx33 movementMtx = CreateTransformMtx(1, 1, 0, 0, 4);
			AEMtx33Concat(&playerMtx, &movementMtx, &playerMtx);
			//playerCenter = getCenterFromCircleMtx(transform);
		}
		if (AEInputCheckCurr(AEVK_S)) {
			//std::cout << "S Pressed\n";
			AEMtx33 movementMtx = CreateTransformMtx(1, 1, 0, 0, -4);
			AEMtx33Concat(&playerMtx, &movementMtx, &playerMtx);
			//playerCenter = getCenterFromCircleMtx(transform);
		}
		if (AEInputCheckCurr(AEVK_A)) {
			//std::cout << "A Pressed\n";
			AEMtx33 movementMtx = CreateTransformMtx(1, 1, 0, -4, 0);
			AEMtx33Concat(&playerMtx, &movementMtx, &playerMtx);
			//playerCenter = getCenterFromCircleMtx(transform);
		}
		if (AEInputCheckCurr(AEVK_D)) {
			//std::cout << "D Pressed\n";
			AEMtx33 movementMtx = CreateTransformMtx(1, 1, 0, 4, 0);
			AEMtx33Concat(&playerMtx, &movementMtx, &playerMtx);
			//playerCenter = getCenterFromCircleMtx(transform);
		}

		// Set the background to black.
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		//Draw the player
		AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 0.0f);
		AEGfxSetTransform(playerMtx.m);
		AEGfxMeshDraw(playerMesh, AE_GFX_MDM_TRIANGLES);



		AEGfxSetCamPosition(250, 250);

		// Informing the system about the loop's end
		AESysFrameEnd();

	}

	// free the system
	AESysExit();
}