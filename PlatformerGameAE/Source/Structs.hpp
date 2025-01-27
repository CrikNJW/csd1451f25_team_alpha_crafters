#pragma once
// For all structs
// when creating variable, keep first letter capital for consistency

enum GroundEnemyState {
	MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_UP
};

struct Player {
	float posX, posY; // position of the player
	float width, height; // width and height of player
	float rotate_angle; //in angle, not radians
	float speed; //  speed of player
	int health; //health and
};

struct Ground_enemy {
	float PosX, PosY;
	float width, height;
	float angle;
	float speed;
	AEMtx33 finalTransform;
	GroundEnemyState state;
};

struct Platform {
	float PosX, PosY;
	float width, height;
	AEMtx33 finalTransform;
};

struct Floating_enemy {
	float PosX, PosY;
	float Width, Height;
	int Health;
};

struct Floating_enemy {
	float PosX, PosY;
	float Width, Height;
	int Health;
};

 struct Boundaries {
	float PosX, PosY; 
	float Width, Height;
}; // this is for the boundaries / borders around map
// Initialize with an array of structs