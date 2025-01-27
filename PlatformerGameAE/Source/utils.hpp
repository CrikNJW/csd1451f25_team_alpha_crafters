#pragma once
#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "Structs.hpp"



AEGfxVertexList* createSquareMesh();

AEMtx33 createTransformMtx(f32 scaleX, f32 scaleY, f32 rotate_rad, f32 translX, f32 translY);

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

void UpdatePlayerPos(Player* player, AEGfxVertexList* player_mesh);

//default player movement
void UpdatePlayerMovement(Player* player, AEGfxVertexList* player_mesh);

//collision check for player and boundary
void CheckCollision(Player& player, const Boundaries& boundary);

void ElasticEnemyCollision(Player& player, Ground_enemy& enemy);

