#include <crtdbg.h> // To check for memory leaks
#include <iostream>
#include <vector>

#include "AEEngine.h"
#include "Structs.hpp"
#include "Block.hpp"

Block::Block(float pos_X, float pos_Y, AEGfxVertexList* mesh) {
	this->id = BLOCK;
	this->mesh = mesh;
	this->posX = pos_X;
	this->posY = pos_Y;
}