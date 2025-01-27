#pragma once
#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "Structs.hpp"



AEGfxVertexList* createSquareMesh();

void InitializePlatform(Platform& platform);

void RenderPlatform(Platform& platform, AEGfxVertexList* mesh);

void UpdateGroundEnemy(Ground_enemy& enemy, Platform& platform, float dt);

void RenderGroundEnemy(Ground_enemy& enemy, AEGfxVertexList* mesh);

