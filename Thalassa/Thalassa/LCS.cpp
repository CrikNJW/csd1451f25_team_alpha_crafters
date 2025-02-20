#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "Structs.hpp"
#include <cmath>
#include <vector>
#include "FloatingEnemy.hpp"
#include "utils.hpp"

/************************************
*									*
*	LEVEL CREATION SYSTEM			*
*									*
*************************************/

/*
* Gets the closest grid coordinate in std::vector grid to the mouse position
* @param grid: The vector of grid coordinates to check
* @param mouseX: The x position of the mouse
* @param mouseY: The y position of the mouse
* @return: The closest grid coordinate to the mouse
*/
GridCoordinate getClosestGridCoordinate(s32 mouseX, s32 mouseY, s32 playerX, s32 playerY, s32 squareGridLength) {
	s32 screenWidth = AEGfxGetWindowWidth();
	s32 screenHeight = AEGfxGetWindowHeight();
	s32 screenCenterX = screenWidth / 2;
	s32 screenCenterY = screenHeight / 2;

	/*Convert screen-space mouse coordinates to world-space coordinates

	E.g. If player is at world-space 0,0, clicking the center of the screen
	would give us screen-space 800,450. We want to convert this to world-space 0,0 as it
	accurately reflects the player's position.

	Solution to get adjusted X: ScreenspaceX(800) - screenCenterX(800) + playerworldspaceX(0) = worldspaceX(0)
	Solution to get adjusted Y: screenCenterY(450) - ScreenspaceY(450) + playerworldspaceY(0) = worldspaceY(0)

	Y-axis is inverted in world-space, screen-space goes from top to bottom, world-space goes from center to top/to bottom.

	E.g. If player is at worldspace (300,200), clicking the center of the screen will give us screenspace(800,450)

	adjustedX = 800 - 800 + 300 = 300
	adjustedY = 450 - 450 + 200 = 200
	*/
	s32 worldMouseX = mouseX - screenCenterX + playerX;
	s32 worldMouseY = screenCenterY - mouseY + playerY;

	/*Snap to the nearest grid coordinate by rounding up or down, this is more performant than
	abusing vectors and iterating through them*/
	s32 gridX = round((float)worldMouseX / squareGridLength) * squareGridLength;
	s32 gridY = round((float)worldMouseY / squareGridLength) * squareGridLength;

	//Create a coordinate to store the closest grid coordinate
	GridCoordinate closestGrid = { gridX, gridY };
	return closestGrid;
}

GridCoordinate handle_LMouseClickInEditor(Player& diver, s32 squareGridLength, int objID, std::vector<Floatie*>& floatingEnemies, AEGfxVertexList* mesh) {
	if (AEInputCheckReleased(AEVK_LBUTTON)) {
		s32 mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);

		//Get the closest grid coordinate to the mouse
		GridCoordinate closestCoord = getClosestGridCoordinate(mouseX, mouseY, (s32)diver.posX, (s32)diver.posY, squareGridLength);

		if (objID == 2) {
			//Create a new floating enemy object at the closest grid coordinate
			Floatie* object = new Floatie(closestCoord.x, closestCoord.y, mesh);
			floatingEnemies.push_back(object);
		}

		//Debugging
		//std::cout << "Closest Coordinate " << closestCoord.x << " " << closestCoord.y << '\n';

		//Return the closest grid coordinate
		return closestCoord;
	}

	return { 0,0 };
}

void PlaceObject(s32 worldSpaceX, s32 worldSpaceY, AEGfxVertexList* mesh) {
	//Create a transform matrix to place the object at the specified world-space coordinates
	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f); // White colour
	AEMtx33 transformMtx = createTransformMtx(50, 50, 0, worldSpaceX, worldSpaceY);
	AEGfxSetTransform(transformMtx.m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetColorToMultiply(0, 0, 0, 0);
}

/************************************
*									*
*	END OF LEVEL CREATION SYSTEM	*
*									*
*************************************/