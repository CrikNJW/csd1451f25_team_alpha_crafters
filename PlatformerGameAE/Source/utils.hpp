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

//default player movement
void UpdatePlayerMovement(Player* player, AEGfxVertexList* player_mesh);

//collision check for player and boundary
void CheckCollision(Player& player, const Boundaries& boundary);

void ElasticEnemyCollision(Player& player, Ground_enemy& enemy);

