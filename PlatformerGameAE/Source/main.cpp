// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "utils.hpp"
#include "Structs.hpp"

u32 white = 0xFFFFFFFF;

f32 speed, rotate_degree;


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
	// Changing the window title
	AESysSetWindowTitle("Thalassa");

	// reset the system modules
	AESysReset();

	// Create a Pointer to Mesh of Rectangle for player texture
	AEGfxVertexList* playerMesh = createSquareMesh();

	//Initialisation of Player Variables
	// Pos X, Pox Y, Width, Height, Rotation degree, Speed, Health
	Player player = { -500.f, -200.f, 50.f, 50.f, 0.f, 0.f, 3 };
	rotate_degree = 2.f;

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		AEGfxSetBackgroundColor(1.0f, 1.0f, 1.0f); // Tell the Alpha Engine to set the background to white.
		player.speed = AEFrameRateControllerGetFrameTime() * 300.f;
		// Tell the Alpha Engine to get ready to draw something.
		AEGfxSetRenderMode(AE_GFX_RM_COLOR); // Draw with Texture /to draw with color, use (AF_GFX_RM_COLOR)

		//PLAYER RENDERING
		//Movement of the Player
		if (AEInputCheckCurr(AEVK_W)) {
			player.posY += player.speed * AESin(AEDegToRad(player.rotate_angle));
			player.posX += player.speed * AECos(AEDegToRad(player.rotate_angle));
		}
		else if (AEInputCheckCurr(AEVK_S)) {
			player.posY -= player.speed * AESin(AEDegToRad(player.rotate_angle));
			player.posX -= player.speed * AECos(AEDegToRad(player.rotate_angle));
		}
		

		if (AEInputCheckCurr(AEVK_LEFT)) {
			player.rotate_angle += 2.f;
			if (player.rotate_angle >= 360.f) {
				player.rotate_angle = 0.f;
			}
		}
		else if (AEInputCheckCurr(AEVK_RIGHT)) {
			player.rotate_angle -= 2.f;
			if (player.rotate_angle < 0) {
				player.rotate_angle = 360.f;
			}
		}
		


		AEGfxSetColorToMultiply(0.5f, 0.5f, 0.5f, 1.0f); // Set to grey colour
		
		//For player Mesh
		AEMtx33 playerMtx = CreateTransformMtx(player.width, player.height, AEDegToRad(player.rotate_angle), player.posX, player.posY);
		AEGfxSetTransform(playerMtx.m);
		AEGfxMeshDraw(playerMesh, AE_GFX_MDM_TRIANGLES);
		
	
		
		
		

		// Basic way to trigger exiting the application
		// when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		// Informing the system about the loop's end
		AESysFrameEnd();

	}
	
	//Free the Mesh
	AEGfxMeshFree(playerMesh);
	// free the system
	AESysExit();
}