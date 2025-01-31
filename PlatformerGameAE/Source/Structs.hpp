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
	f32 posX, posY; // position of the player
	float width, height; // width and height of player
	float rotate_angle; //in angle, not radians
	f32 speed; //  speed of player
	int health; //health and
	int maxhealth; // player max health
	bool lockMovement = false; //Prevent player from moving and rotating
	f32 lockTimeElapsed = 0; //Time elapsed for icicle drop
	f32 lockTime = 1; //Time to lock player movement
	void takedamage(int damage) {
		health -= damage;
		if (health < 0) health = 0; // make sure the health doesnt go below 0
	}
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

struct Boundaries {
	float PosX, PosY; 
	float Width, Height;
}; // this is for the boundaries / borders around map
// Initialize with an array of structs

struct Icicle {
	float PosX, PosY;
	float dropOffsetY = 0;
	float dropSpeed = 70;
	float childX = PosX;
	float childY = PosY;
	float timeElapsed = 0;
	float cooldown = 2;
	float cooldownElapsed = 0;
	Boundaries boundaries = {PosX, PosY, 40, 40};
};

struct LavaSpout {
	float PosX, PosY;   // Volcano position
	float lavaX, lavaY;  // Lava drop position
	float velocityX, velocityY; // For parabolic motion
	float dropSpeed = 120.0f; // <-- Increase this for a faster fall
	float cooldown = 2.0f; // Time before respawning
	float cooldownElapsed = 0.0f;
	float timeElapsed = 0.0f;
	bool isActive = false;

	LavaSpout(float x, float y) {
		PosX = x;
		PosY = y;
		lavaX = PosX;
		lavaY = PosY;
		velocityX = 0;
		velocityY = 0;
	}
};

