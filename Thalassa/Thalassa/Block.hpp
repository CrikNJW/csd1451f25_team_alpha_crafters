#pragma once

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "Structs.hpp"
#include <iostream>
#include <vector>

class Block : public GameObject { //Block class inherits from GameObject struct
public:
	Boundaries boundaries = {0,0,50,50};

	Block(float pos_X, float pos_Y, AEGfxVertexList* mesh); //Constructor
};