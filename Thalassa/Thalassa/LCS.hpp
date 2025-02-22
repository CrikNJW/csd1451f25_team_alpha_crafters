#pragma once

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "Structs.hpp"
#include "FloatingEnemy.hpp"
#include <vector>

GridCoordinate getClosestGridCoordinate(s32 mouseX, s32 mouseY, s32 playerX, s32 playerY, s32 squareGridLength);
void LCS_HandleMouseClick(Player& diver, s32 squareGridLength, int& LCS_ObjID, std::vector<GameObject*>& LCS_GameObjects, std::vector<Boundaries>& LCS_Boundaries, AEGfxVertexList* mesh);
void PlaceObject(s32 worldSpaceX, s32 worldSpaceY, AEGfxVertexList* mesh);
void LCS_Trigger(bool& LCS_Mode);
void LCS_KeyTrigger(AEGfxVertexList*& LCS_SelectedMesh, int& LCS_ObjID, int& LCS_CurrKey, std::vector<AEGfxVertexList*> LCS_Meshes);
