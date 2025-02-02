#pragma once
#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "Structs.hpp"
#include <vector>



AEGfxVertexList* createSquareMesh();
AEGfxVertexList* createCircleMesh();
AEMtx33 createTransformMtx(f32 scaleX, f32 scaleY, f32 rotate_rad, f32 translX, f32 translY);
std::vector<GridCoordinate> initializeGridSystem(s32 squareGridLength);
GridCoordinate getClosestGridCoordinate(const std::vector<GridCoordinate>& grid, s32 mouseX, s32 mouseY, s32 playerX, s32 playerY);
GridCoordinate handle_LMouseClickInEditor(const std::vector<GridCoordinate>& grid, Player& diver);

void DrawIcicle(Icicle &icicle, AEGfxVertexList* icicleMesh);
void Draw_UpdateIcicleDrop(Icicle &icicle, AEGfxVertexList* icicleMesh, f32 dt);
bool icicleCollision(Player& player, Icicle& icicle);
void initIcicle(Icicle& icicle);

//int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

void DrawBlackOverlay(AEGfxVertexList* square_mesh, Player& player);

void SpotLight(Player* player, AEGfxVertexList* circle_mesh);
void UpdatePlayerPos(Player* player, AEGfxVertexList* player_mesh, f32 dt);

void PlayerDash(Player* player, AEGfxVertexList* CooldownMesh, f32 dt);

void CheckCollision(Player& player, const Boundaries& boundary);

void ElasticEnemyCollision(Player& player, f32 enemy_x, f32 enemy_y, f32 enemy_width, f32 enemy_height);

void InitializeBoundary(Boundaries& platform);

void UpdateGroundEnemy(Ground_enemy& enemy, Boundaries& boundary, float dt);

void RenderGroundEnemy(Ground_enemy& enemy, AEGfxVertexList* mesh);

void RenderHealthBar(const Player& player, AEGfxVertexList* mesh);

bool lavaCollision(Player& player, LavaSpout& lavaSpout);

void Draw_UpdateLavaDrop(LavaSpout& lavaSpout, AEGfxVertexList* lavaMesh, float dt);

void UpdateBurrowingEnemy(Burrowing_enemy& enemy, float playerX, float playerY, float dt);

void RenderBurrowingEnemy(Burrowing_enemy& enemy, AEGfxVertexList* mesh);

void RenderBoundary(Boundaries& boundary, AEGfxVertexList* platformMesh);