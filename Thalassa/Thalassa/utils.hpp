#pragma once
#include <crtdbg.h> // To check for memory leaks
#include <iostream>
#include <vector>

#include "AEEngine.h"
#include "Structs.hpp"
#include "Enemies.hpp"
#include "Hazards.hpp"
#include "Player.hpp"
#include "LCS.hpp"

AEGfxVertexList* createSquareMesh();
AEGfxVertexList* createCircleMesh();
AEMtx33 createTransformMtx(f32 scaleX, f32 scaleY, f32 rotate_rad, f32 translX, f32 translY);

void DrawIcicle(Icicle &icicle, AEGfxVertexList* icicleMesh);
void Draw_UpdateIcicleDrop(Icicle &icicle, AEGfxVertexList* icicleMesh, f32 dt);
bool icicleCollision(Player& player, Icicle& icicle);
void initIcicle(Icicle& icicle);

//int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
int InCircleRadius(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);
int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

void DrawBlackOverlay(AEGfxVertexList* square_mesh, Player& player, LavaSpout& lavaSpout);
void SpotLight(Player* player, AEGfxVertexList* circle_mesh);

void UpdatePlayerPos(Player* player, AEGfxVertexList* player_mesh, f32 dt);

void PlayerDash(Player* player, AEGfxVertexList* CooldownMesh, f32 dt);

void CheckCollision(Player& player, const Boundaries& boundary);
void ElasticEnemyCollision(Player& player, f32 enemy_x, f32 enemy_y, f32 enemy_width, f32 enemy_height);

void InitializeBoundary(Boundaries& platform);
void RenderBoundary(Boundaries& boundary, AEGfxVertexList* platformMesh);

void UpdateGroundEnemy(Ground_enemy& enemy, Boundaries& boundary, float dt);
void RenderGroundEnemy(Ground_enemy& enemy, AEGfxVertexList* mesh);

void RenderHealthBar(const Player& player, AEGfxVertexList* mesh);

bool lavaCollision(Player& player, LavaSpout& lavaSpout);
void Draw_UpdateLavaDrop(LavaSpout& lavaSpout, AEGfxVertexList* lavaMesh, float dt);

void UpdateBurrowingEnemy(Burrowing_enemy& enemy, float playerX, float playerY, float dt);
void RenderBurrowingEnemy(Burrowing_enemy& enemy, AEGfxVertexList* mesh);

void RenderBlocks(std::vector<GameObject*>& LCS_GameObjects, Player& diver, f32 dt);
void RenderFloatingEnemies(std::vector<GameObject*>& LCS_GameObjects, Player& diver, f32 dt);
void UpdateFloatingEnemies(std::vector<GameObject*>& LCS_GameObjects, Player& diver, f32 dt);