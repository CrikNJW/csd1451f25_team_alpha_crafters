#pragma once

// For all structs
// when creating variable, keep first letter capital for consistency
enum GameObjectIDs {
	BLOCK = 1,
	ICICLE,
	LAVA_SPOUT,
	SEAWEED,
	GROUND_ENEMY,
	BURROWING_ENEMY,
	FLOATING_ENEMY
};

struct GridCoordinate {
	s32 x, y;
};

struct GameObject { //Base parent class for ALL game objects
	int id;
	f32 posX, posY;
	AEGfxVertexList* mesh;
	GridCoordinate gridPos;
};

struct Boundaries {
	float PosX, PosY; 
	float Width, Height;
	AEMtx33 finalTransform;
}; // this is for the boundaries / borders around map
// Initialize with an array of structs
