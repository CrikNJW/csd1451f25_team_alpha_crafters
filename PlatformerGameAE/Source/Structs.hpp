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
	//for player dash properties
	bool isDashing; // Flag to check if player is currently dashing
	float dashSpeed; // How fast the dash moves
	float dashDuration; // How long the dash lasts
	float dashCooldown; // Time before player can dash again
	float currentDashTime; // Track current dash duration
	float dashCooldownTimer; // Track cooldown time
	bool lockMovement = false; //Prevent player from moving and rotating
	f32 lockTimeElapsed = 0; //Time elapsed for icicle drop
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

struct Icicle {
	float PosX, PosY;
	float dropOffsetY = 0;
	float dropSpeed = 70;
	float childX = PosX;
	float childY = PosY;
	float timeElapsed = 0;
	float cooldown = 2;
	float cooldownElapsed = 0;
};

struct Boundaries {
	float PosX, PosY; 
	float Width, Height;
}; // this is for the boundaries / borders around map
// Initialize with an array of structs

