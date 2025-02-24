#pragma once
#include <AEEngine.h>

const float DIVER_ACCEL = 100.f; //player acceleration (in m/s^2)

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