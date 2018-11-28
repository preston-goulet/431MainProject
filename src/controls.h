/**
 * SER 431
 * https://speakerdeck.com/javiergs/ser431-lecture-04
 **/

#pragma once

#include <GL/glut.h>
#include "GameObject.h"
#include "Camera.h"

int window_width = 800, window_height = 800;
float window_ratio = 1.0;

// controling parameters
int mouse_button;
int mouse_x = 0;
int mouse_y = 0;
float scale = 0.3;
float x_angle = 0.0;
float y_angle = 0.0;

int totalScore = 00;
int countDown = 10000;
int moveSpeed = 500;
float playerLook = 0;
float angle = 0;

bool raiseAndLower = false;
bool getInBox = true;
bool doOnce = true;

float total_moving_angle = 0.0;

const int perlinMeshSize = 80000;

float meshSize = (perlinMeshSize - 1) / 2;
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

float moveBlock = 0.0;
float moveBlock_side = 0.0;

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

void updateBoxPositon(Mesh* mesh, int xOffset, int zOffset) {
	for (Vec3f i : mesh->dot_vertex) {
		float x = jet.position[0];
		float z = jet.position[2];
		if (abs(x - (i.x - xOffset)) < 200 && abs(z - (i.z - zOffset)) < 200) {
			jet.position[1] = 10 + i.y;
		}
	}
}

// callback function for keyboard (alfanumeric keys)
void callbackKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': case 'W':
		looky += .1;
		break;
	case 's': case 'S':
		looky -= .1;
		break;
	case 'a': case 'A':
		camera_y -= 50;
		break;
	case 'd': case 'D':
		camera_y += 50;
		break;
	case 'l': case 'L':
		if (camera_x > -895 && camera_x < 655 && camera_z > -16890 && camera_z < -14800 && doOnce == true) {
			if (leftBox) {
				correctChoice = true;
				totalScore++;
				doOnce = false;
				raiseAndLower = true;
				getInBox = false;
				break;
			}
			else {
				correctChoice = false;
				raiseAndLower = true;
				getInBox = false;
				break;
			}
		}
		else {
			getInBox = true;
		}
		break;
	case 'r': case 'R':
		if (camera_x > -895 && camera_x < 655
			&& camera_z > -16890 && camera_z < -14800 && doOnce == true) {
			if (!leftBox) {
				correctChoice = true;
				totalScore++;
				doOnce = false;
				raiseAndLower = true;
				getInBox = false;
				break;
			}

			else {
				correctChoice = false;
				raiseAndLower = true;
				getInBox = false;
				break;
			}
		}
		else {
			getInBox = true;
		}
		break;

	}

	if (jet.position[0] > meshSize) {
		jet.position[0] = meshSize;
	}
	else if (jet.position[0] < -meshSize) {
		jet.position[0] = -meshSize;
	}

	// box Z verification
	if (jet.position[2] > meshSize) {
		jet.position[2] = meshSize;
	}
	else if (jet.position[2] < -meshSize) {
		jet.position[2] = -meshSize;
	}
}

void specialkeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		moveMeFlat(moveSpeed);
		break;
	case GLUT_KEY_DOWN:
		moveMeFlat(-moveSpeed);
		break;
	case GLUT_KEY_LEFT:
		angle -= 0.1f;
		orientMe(angle);
		playerLook += 5;

		break;
	case GLUT_KEY_RIGHT:
		angle += 0.1f;
		orientMe(angle);
		playerLook -= 5;

		break;
	}

	glutPostRedisplay();

}