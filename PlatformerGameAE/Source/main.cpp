// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "utils.hpp"
#include "Structs.hpp"

u32 white = 0xFFFFFFFF;

f32 speed, rotate_speed;
f32 player_w, player_h, player_x, player_y, player_rotation;


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

	//Initialisation of Player Variables
	player_w = 50.f;
	player_h = 50.f;
	player_x = -500.f; //starting x position
	player_y = -200.f; //starting y position
	player_rotation = 0.f;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	AEGfxVertexList* playerMesh = createSquareMesh();
	

	// Changing the window title
	AESysSetWindowTitle("Thalassa");

	// reset the system modules
	AESysReset();

	// Create a Pointer to Mesh of Rectangle for player texture
	AEGfxVertexList* squareMesh = 0;

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		AEGfxSetBackgroundColor(1.0f, 1.0f, 1.0f); // Tell the Alpha Engine to set the background to white.
		speed = AEFrameRateControllerGetFrameTime() * 300.f;
		rotate_speed = AEFrameRateControllerGetFrameTime();
		// Tell the Alpha Engine to get ready to draw something.
		AEGfxSetRenderMode(AE_GFX_RM_COLOR); // Draw with Texture /to draw with color, use (AF_GFX_RM_COLOR)

		//PLAYER RENDERING
		//Movement of the Player
		if (AEInputCheckCurr(AEVK_W)) {
			player_y += speed;
		}
		else if (AEInputCheckCurr(AEVK_S)) {
			player_y -= speed;
		}
		else if (AEInputCheckCurr(AEVK_A)) {
			player_x -= speed;
		}
		else if (AEInputCheckCurr(AEVK_D)) {
			player_x += speed;
		}

		if (AEInputCheckCurr(AEVK_LEFT)) {
			player_rotation += 0.1f;
			/*if (player_rotation >= 360.f) {
				player_rotation = 0.f;
			}*/
		}
		else if (AEInputCheckCurr(AEVK_RIGHT)) {
			player_rotation -= 0.1f;
		}
		


		AEGfxSetColorToMultiply(0.5f, 0.5f, 0.5f, 1.0f); // Set to grey colour
		
		//For player Mesh
		AEMtx33 playerMtx = CreateTransformMtx(player_w, player_h, player_rotation, player_x, player_y);
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