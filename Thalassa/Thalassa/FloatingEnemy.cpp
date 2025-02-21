#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "Structs.hpp"
#include <iostream>
#include <vector>
#include "FloatingEnemy.hpp"

//Constructor
Floatie::Floatie(float pos_X, float pos_Y, AEGfxVertexList* mesh) {
	gameobj.id = 2;
	gameobj.mesh = mesh;
	gameobj.posX = pos_X;
	gameobj.posY = pos_Y;
}

void Floatie::CheckDistance(Player& player) {
	//Get the distance between the player and the object
	float distance = (float)sqrt(pow(player.posX - gameobj.posX, 2) + pow(player.posY - gameobj.posY, 2));
	if (distance <= 300) {
		currentState = CHASE;
	}
	else {
		currentState = IDLE;
	}
}

//Game Logic
void Floatie::Update(Player& player, f32 dt) {
	if (currentState == CHASE) {
		speed = 100;
		//Move towards player
		f32 angle = atan2(player.posY - gameobj.posY, player.posX - gameobj.posX);
		gameobj.posX += cos(angle) * speed * dt;
		gameobj.posY += sin(angle) * speed * dt;
	}
}

void Floatie::IdleMovement(f32 dt) {
	if (currentState == IDLE)
	{
		idleTimer -= dt;

		// If timer runs out, pick a new direction or stop moving
		if (idleTimer <= 0) {
			int action = rand() % 3; // 0: stop, 1: change direction, 2: keep moving

			if (action == 0) {
				speed = 0; // Stop moving
			}
			else {
				moveAngle = (rand() % 360) * (PI / 180.0f); // Generate random angle
				speed = 50; // Resume movement
			}

			// Reset the timer to a random value between 3 and 7 seconds
			idleTimer = 3.0f + static_cast<float>(rand() % 5);
		}

		// Move in the current direction if not stopped
		gameobj.posX += cos(moveAngle) * speed * dt;
		gameobj.posY += sin(moveAngle) * speed * dt;
	}
}

void Floatie::IncrTimer(f32 dt) {
	idleTimer += dt;
}