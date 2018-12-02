#pragma once

#include <math.h>

// camera
float camera_x = 0.0;
float camera_y = 500.0;
float camera_z = 600.0;
float camera_rotate = 0;
float camera_viewing_x = 0.0;
float camera_viewing_y = 300.0;
float camera_viewing_z = 0.0;

//Moving Flat
float lookx = 0.0f, looky = 0.0f, lookz = -1.0f;

// callback function for arrows
void moveMeFlat(int i) {
	if (camera_x <= 37726 && camera_x >= -38833 && camera_z <= 37301 && camera_z >= -37451) {
		camera_x = camera_x + i * (lookx)*1.0;
		camera_z = camera_z + i * (lookz)*1.0;
	}
	else {
		camera_x = 0;
		camera_z = 600;
	}
	
}

void orientMe(float ang) {
	lookx = sin(ang);
	lookz = -cos(ang);
}

void updateCamera() {
	if(isGameModeOn) {
		camera_y = 500;
	}
}