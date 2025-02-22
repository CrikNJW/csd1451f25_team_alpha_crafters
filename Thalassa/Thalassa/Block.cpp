#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "Structs.hpp"
#include <iostream>
#include <vector>
#include "Block.hpp"

Block::Block(float pos_X, float pos_Y, AEGfxVertexList* mesh) {
	this->id = BLOCK;
	this->mesh = mesh;
	this->posX = pos_X;
	this->posY = pos_Y;
}