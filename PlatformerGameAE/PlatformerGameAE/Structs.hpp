#pragma once
// For all structs
// when creating variable, keep first letter capital for consistency


typedef struct Player {
	float PosX, PosY; // position of the player
	float Width, Height; // width and height of player
	int Health, Speed; // health and speed of player
};

typedef struct Ground_Enemy {
	float PosX, PosY;
	float Width, Height;
	int Health;
};

typedef struct Floating_Enemy {
	float PosX, PosY;
	float Width, Height;
	int Health;
};

typedef struct Boundaries {
	float PosX, PosY; 
	float Width, Height;
}; // this is for the boundaries / borders around map
// Initialize with an array of structs