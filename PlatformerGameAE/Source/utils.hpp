#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);