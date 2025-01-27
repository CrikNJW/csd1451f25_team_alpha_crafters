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
	float Width, Height;
	float angle;
	float speed;
	AEMtx33 finalTransform;
	GroundEnemyState state;
};

struct Platform {
	float PosX, PosY;
	float Width, Height;
	AEMtx33 finalTransform;
};

struct Floating_enemy {
	float PosX, PosY;
	float Width, Height;
	int Health;
};

<<<<<<< HEAD


 struct Boundaries {
=======
struct Boundaries {
>>>>>>> parent of c81e7e0 (Merge branch 'main' into Trisha-GroundEnemyAI)
	float PosX, PosY; 
	float Width, Height;
}; // this is for the boundaries / borders around map
// Initialize with an array of structs