#pragma once
// For all structs
// when creating variable, keep first letter capital for consistency


typedef struct player {
	float PosX, PosY; // position of the player
	float Width, Height; // width and height of player
	int Health, Speed; // health and speed of player
};

typedef struct ground_enemy {
	float PosX, PosY;
	float Width, Height;
	int Health;
};

typedef struct floating_enemy {
	float PosX, PosY;
	float Width, Height;
	int Health;
};

typedef struct boundaries {
	float PosX, PosY; 
	float Width, Height;
}; // this is for the boundaries / borders around map
// Initialize with an array of structs