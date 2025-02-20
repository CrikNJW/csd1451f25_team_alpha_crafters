#pragma once

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "Structs.hpp"
#include <iostream>

class Floatie {
public:
	enum FloatingEnemyStates {
		IDLE,
		CHASE,
	};

	GameObject gameobj;
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


void UpdateFloatingEnemies(std::vector<Floatie*>& floatingEnemies, Player& diver, f32 dt) {
	for (int i = 0; i < floatingEnemies.size(); i++) {
		PlaceObject(floatingEnemies[i]->gameobj.posX, floatingEnemies[i]->gameobj.posY, floatingEnemies[i]->gameobj.mesh);
		floatingEnemies[i]->CheckDistance(diver);
		floatingEnemies[i]->Update(diver, dt);
		floatingEnemies[i]->IdleMovement(dt);
	}
}