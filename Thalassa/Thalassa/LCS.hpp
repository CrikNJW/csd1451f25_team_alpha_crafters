#pragma once

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "Structs.hpp"
#include "FloatingEnemy.hpp"
#include <vector>

GridCoordinate getClosestGridCoordinate(s32 mouseX, s32 mouseY, s32 playerX, s32 playerY, s32 squareGridLength);
GridCoordinate handle_LMouseClickInEditor(Player& diver, s32 squareGridLength, int objID, std::vector<Floatie*>& floatingEnemies, AEGfxVertexList* mesh);
void PlaceObject(s32 worldSpaceX, s32 worldSpaceY, AEGfxVertexList* mesh);