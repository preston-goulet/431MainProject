/**
 * SER 431
 * controls.h
 **/

#pragma once

#include <math.h>
#include <GL/glut.h>
#include <cstdlib>

int window_width = 800, window_height = 800;
float window_ratio = 1.0;

// controling parameters
int mouse_button;
int mouse_x = 0;
int mouse_y = 0;
float scale = 0.3;
float x_angle = 0.0;
float y_angle = 0.0;

// camera
float camera_x = 0.0;
float camera_y = 500.0;
float camera_z = 600.0;

float camera_rotate = 0;
float jet_rotate = 0;

float camera_viewing_x = 0.0;
float camera_viewing_y = 300.0;
float camera_viewing_z = 0.0;

float total_moving_angle = 0.0;

//Moving box values
float jetPositionX = 0.0;
float jetPositionY = 100.0;
float jetPositionZ = 0.0;

const int perlinMeshSize = 80000;

float meshSize = (perlinMeshSize - 1) / 2;
int cameraSpeed = 1;
int boxSpeed = 10;

float runway_x = -950;
float runway_y = 150;
float runway_z = -1000;

int shadowHeight = 200;
int playArea = -16000;
bool leftBox = false; // Random number will be even for left box or odd for right
char usersChoice = 'l'; // user will choose l or r for left or right box
bool correctChoice = false; // If the user guesses the correct box then true
bool lowerBox = false; //Determines if box needs lowered after being raised
int boxMovement = 0; //Added to the y value of the boxes when they need to be raised and lowered


void updateBoxPositon(Mesh* mesh, int xOffset, int zOffset) {
	for (Vec3f i : mesh->dot_vertex) {
		if (abs(jetPositionX - (i.x - xOffset)) < 200 && abs(jetPositionZ - (i.z - zOffset)) < 200) {
			jetPositionY = 10 + i.y;
		}
	}
}

// Generates a random number to see if its even or odd
// If even, the left box is the correct box
void generateRandomNumber() {
	int r = (rand() % 100) + 1;
	if (r % 2 == 0) {
		leftBox = true;
	}
	else {
		leftBox = false;
	}
}

float moveBlock = 0.0;
float moveBlock_side = 0.0;
