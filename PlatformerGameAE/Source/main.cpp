// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "utils.hpp"

u32 white = 0xFFFFFFFF;

f32 speed;
f32 player_w, player_h, player_x, player_y;


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

	player_w = 100.f;
	player_h = 100.f;
	player_x = 0.f;
	player_y = 0.f;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);
	// Changing the window title
	AESysSetWindowTitle("My New Demo!");
	// reset the system modules
	AESysReset();

	// Create a Pointer to Mesh of Rectangle for texture
	AEGfxVertexList* squareMesh = 0;
	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();
	//Triangle A that left side is start from origin
	AEGfxTriAdd(
		0.f, 0.f, white, 0.0f, 1.0f,
		0.f, 1.0f, white, 1.0f, 1.0f,
		1.0f, 0.f, white, 0.0f, 0.0f);
	//Triangle B that form a square with triangle A
	AEGfxTriAdd(
		0.f, 1.0f, white, 1.0f, 1.0f,
		1.0f, 1.0f, white, 1.0f, 0.0f,
		1.0f, 0.f, white, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in healthMesh that we created earlier
	squareMesh = AEGfxMeshEnd();


	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		AEGfxSetBackgroundColor(1.0f, 1.0f, 1.0f); // Tell the Alpha Engine to set the background to white.
		speed = AEFrameRateControllerGetFrameRate() / 10.f;
		

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
		
		AEMtx33 scale_player = { 0 };
		AEMtx33Scale(&scale_player, player_w, player_h);

		AEMtx33 translate_player = { 0 }; //translate up
		AEMtx33Trans(&translate_player, player_x, player_y); //second param is x-axis, third param is y-axis

		AEMtx33 transform_player = { 0 }; //Transform to top of screen
		AEMtx33Concat(&transform_player, &translate_player, &scale_player); //put the translate and scale matrix into transform (scale first then translate)


		AEGfxSetColorToMultiply(0.5f, 0.5f, 0.5f, 1.0f); // Set to grey colour
		AEGfxSetTransform(transform_player.m);
		AEGfxMeshDraw(squareMesh, AE_GFX_MDM_TRIANGLES);
		
		
		
		
		
		
		
		// Basic way to trigger exiting the application
		// when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		

	

		// Informing the system about the loop's end
		AESysFrameEnd();

	}

	// free the system
	AESysExit();
}