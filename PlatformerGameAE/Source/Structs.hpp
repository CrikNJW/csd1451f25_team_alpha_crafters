#pragma once
// For all structs
// when creating variable, keep first letter capital for consistency


typedef struct Player {
	float posX, posY; // position of the player
	float width, height; // width and height of player
	float rotate_angle; //in angle, not radians
	float speed; //  speed of player
	int health; //health and
};

typedef struct Ground_enemy {
	float PosX, PosY;
	float Width, Height;
	int Health;
};

typedef struct Floating_enemy {
	float PosX, PosY;
	float Width, Height;
	int Health;
};

typedef struct Icicle {
	float PosX, PosY;
	float dropOffsetY = 0;
	float dropSpeed = 70;
	float childX = PosX;
	float childY = PosY;
	float timeElapsed = 0;
};

typedef struct Boundaries {
	float PosX, PosY; 
	float Width, Height;
}; // this is for the boundaries / borders around map
// Initialize with an array of structs