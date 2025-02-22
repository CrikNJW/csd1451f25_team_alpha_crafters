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
	f32 lockTime = 1; //Time to lock player movement

	void takedamage(int damage) {
		health -= damage;
		if (health < 0) health = 0; // make sure the health doesnt go below 0
	}
};


struct Boundaries {
	float PosX, PosY; 
	float Width, Height;
	AEMtx33 finalTransform;
}; // this is for the boundaries / borders around map
// Initialize with an array of structs

struct Icicle {
	f32 PosX = 0;
	f32 PosY = 0;
	float dropOffsetY = 0;
	float dropSpeed = 70;
	float childX = 0;
	float childY = 0;
	float timeElapsed = 0;
	float cooldown = 2;
	float cooldownElapsed = 0;
	Boundaries boundaries = { 0, 0, 40, 40};
};

struct LavaSpout {
	float PosX, PosY;   // Volcano position
	float lavaX, lavaY;  // Lava drop position
	float velocityX, velocityY; // For parabolic motion
	float dropSpeed = 120.0f; // Speed of falling lava
	float cooldown = 1.0f; // Time before respawning
	float cooldownElapsed = 0.0f;
	float timeElapsed = 0.0f;
	bool isActive = false;  // Controls visibility and movement

	LavaSpout(float x, float y) {
		PosX = x;
		PosY = y;
		lavaX = PosX;
		lavaY = PosY;
		velocityX = 0;
		velocityY = 0;
	}
};

struct Ground_enemy {
	float PosX, PosY;
	float Width, Height;
	float angle;
	float speed;
	AEMtx33 finalTransform;

	enum GroundEnemyMovement { // Ground enemy movement
		MOVE_RIGHT,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_UP
	};
	GroundEnemyMovement MovementState;

	enum GroundEnemyState {
		IDLE,
		TRACKING,
		ATTACKING,
		RETREATING
	};
	GroundEnemyState State;
};

struct Burrowing_enemy {
	float PosX, PosY;
	float Width, Height;
	float speed;
	float detectionRadius;
	float attackCooldown;
	bool isVisible;
	float alertTimer;  // Time before popping out
	bool spawningParticles; // Indicates if particles are active

	enum BurrowingEnemyState {
		IDLE,
		ALERT,
		ATTACKING,
		RETREATING,
		WAITING
	};
	BurrowingEnemyState State;

	Boundaries* boundary;
};

/*LavaSpout dirtParticles;*/