#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	// Reason why use half width and height is because the rectangle button was drew from the middle
	float half_width = area_width / 2.0;
	float half_height = area_height / 2.0;

	// Compute the boundaries of the button rectangle
	float left_bound = area_center_x - half_width;
	float right_bound = area_center_x + half_width;
	float top_bound = area_center_y - half_height;
	float bottom_bound = area_center_y + half_height;

	// Check if the Mouse_Input is within the boundaries
	if (click_x >= left_bound && click_x <= right_bound && click_y >= top_bound && click_y <= bottom_bound) {
		return 1;
	}
	else {
		return 0;
	}
}

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	float radius = diameter / 2.0;
	// distance = sgrt of [(X1 - X2)^2 + (Y1 - Y2)^2], we eliminate the sqrt by squaring the radius later
	float distance_squared = (click_x - circle_center_x) * (click_x - circle_center_x) + (click_y - circle_center_y) * (click_y - circle_center_y);

	if (distance_squared <= radius * radius) {
		return 1;
	}
	else {
		return 0;
	}
}

int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2)
{
	float collide_dist_sqr = (c1_x - c2_x) * (c1_x - c2_x) + (c1_y - c2_y) * (c1_y - c2_y);

	if (collide_dist_sqr <= (r1 + r2) * (r1 + r2)) {
		return 1;
	}
	else {
		return 0;
	}
}

