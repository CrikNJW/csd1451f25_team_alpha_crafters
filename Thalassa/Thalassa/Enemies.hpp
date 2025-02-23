#pragma once
//Include Standard Libraries
#include <AEEngine.h>
#include <vector>
#include <iostream>

//Include created hpps
#include "Structs.hpp"
#include "Player.hpp"

//Groudn Enemy Struct
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

//Burrowing Enemy Struct
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

//Floating Enemy Class
class Floatie : public GameObject { //Floatie class inherits from GameObject struct
public:
	enum FloatingEnemyStates {
		IDLE,
		CHASE,
		LCS_MODE
	};

	int health = 1;
	int damage = 1;
	float speed = 100;
	int currentState = IDLE;
	f32 idleTimer = 0;
	f32 moveAngle = 0;
	bool isMoving = false;

	Floatie(float pos_X, float pos_Y, AEGfxVertexList* mesh); //Constructor
	void IncrTimer(f32 dt); //Increment the timer
	void CheckDistance(Player& player);	//Check Distance between player and object, triggers the floatie if within distance
	void Update(Player& player, f32 dt); //Update the floatie's position based on player position
	void IdleMovement(f32 dt); //Move the floatie in a random direction
private:
};