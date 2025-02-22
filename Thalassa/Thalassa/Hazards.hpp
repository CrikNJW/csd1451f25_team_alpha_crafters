#pragma once
#include <AEEngine.h>
#include "Structs.hpp"

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
	Boundaries boundaries = { 0, 0, 40, 40 };
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