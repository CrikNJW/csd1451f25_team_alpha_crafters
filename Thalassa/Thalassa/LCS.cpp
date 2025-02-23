#include <crtdbg.h> // To check for memory leaks
#include <iostream>
#include <cmath>
#include <vector>

#include "AEEngine.h"
#include "Structs.hpp"
#include "Enemies.hpp"
#include "Block.hpp"
#include "Hazards.hpp"
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
	s32 gridX = (s32)(round((float)worldMouseX / squareGridLength) * squareGridLength);
	s32 gridY = (s32)(round((float)worldMouseY / squareGridLength) * squareGridLength);

	//Create a coordinate to store the closest grid coordinate
	GridCoordinate closestGrid = { gridX, gridY };
	return closestGrid;
}

void LCS_HandleMouseClick(Player& diver, s32 squareGridLength, int& LCS_ObjID, std::vector<GameObject*>& LCS_GameObjects, std::vector<Boundaries>& LCS_Boundaries, AEGfxVertexList* mesh) {
	if (AEInputCheckReleased(AEVK_LBUTTON)) {
		s32 mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);

		//Get the closest grid coordinate to the mouse
		GridCoordinate closestCoord = getClosestGridCoordinate(mouseX, mouseY, (s32)diver.posX, (s32)diver.posY, squareGridLength);

		switch (LCS_ObjID) {
			case BLOCK: { //ObjID 1 matches square block
				Block* block = new Block(closestCoord.x, closestCoord.y, mesh);
				block->boundaries = { (float)closestCoord.x, (float)closestCoord.y, block->boundaries.Width, block->boundaries.Height };
				LCS_GameObjects.push_back(block);
				LCS_Boundaries.push_back(block->boundaries);
				break;
			}

			case ICICLE: { //ObjID 2 matches icicle
				break;
			}

			case LAVA_SPOUT: { //ObjID 3 matches lava spout
				break;
			}

			case SEAWEED: { //ObjID 4 matches seaweed
				break;
			}

			case GROUND_ENEMY: { //ObjID 5 matches ground enemy
				break;
			}

			case BURROWING_ENEMY: { //ObjID 6 matches burrowing enemy
				break;
			}

			case FLOATING_ENEMY: { //ObjID 7 matches floating enemy
				Floatie* floatie = new Floatie(closestCoord.x, closestCoord.y, mesh);
				LCS_GameObjects.push_back(floatie);
				break;
			}
		}
	}
}

void PlaceObject(s32 worldSpaceX, s32 worldSpaceY, AEGfxVertexList* mesh) {
	//Create a transform matrix to place the object at the specified world-space coordinates
	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f); // White colour
	AEMtx33 transformMtx = createTransformMtx(50, 50, 0, worldSpaceX, worldSpaceY);
	AEGfxSetTransform(transformMtx.m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetColorToMultiply(0, 0, 0, 0);
}

void LCS_Trigger(bool& LCS_Mode) {
	//Trigger Level Creation System
	if (AEInputCheckTriggered(AEVK_L)) {
		LCS_Mode = !LCS_Mode;
	}
}

void LCS_KeyTrigger(AEGfxVertexList*& LCS_SelectedMesh, int& LCS_ObjID, int& LCS_CurrKey, std::vector<AEGfxVertexList*> LCS_Meshes) {
	if (AEInputCheckTriggered(AEVK_1)) { //Square Block
		LCS_SelectedMesh = LCS_Meshes[BLOCK - 1];
		LCS_ObjID = 1;
		LCS_CurrKey = 1;
	}
	else if (AEInputCheckTriggered(AEVK_2)) { //Icicle
		LCS_SelectedMesh = LCS_Meshes[ICICLE - 1];
		LCS_ObjID = 2;
		LCS_CurrKey = 2;
	}
	else if (AEInputCheckTriggered(AEVK_3)) { //Lava Spout
		LCS_SelectedMesh = LCS_Meshes[LAVA_SPOUT - 1];
		LCS_ObjID = 3;
		LCS_CurrKey = 3;
	}
	else if (AEInputCheckTriggered(AEVK_4)) { //Seaweed
		LCS_SelectedMesh = LCS_Meshes[SEAWEED - 1];
		LCS_ObjID = 4;
		LCS_CurrKey = 4;
	}
	else if (AEInputCheckTriggered(AEVK_5)) { //Ground Enemy
		LCS_SelectedMesh = LCS_Meshes[GROUND_ENEMY - 1];
		LCS_ObjID = 5;
		LCS_CurrKey = 5;
	}
	else if (AEInputCheckTriggered(AEVK_6)) { //Burrowing Enemy
		LCS_SelectedMesh = LCS_Meshes[BURROWING_ENEMY - 1];
		LCS_ObjID = 6;
		LCS_CurrKey = 6;
	}
	else if (AEInputCheckTriggered(AEVK_7)) { //Floating Enemy
		LCS_SelectedMesh = LCS_Meshes[FLOATING_ENEMY - 1];
		LCS_ObjID = 7;
		LCS_CurrKey = 7;
	}
}

/************************************
*									*
*	END OF LEVEL CREATION SYSTEM	*
*									*
*************************************/