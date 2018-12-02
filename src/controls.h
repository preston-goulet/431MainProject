/**
 * SER 431
 * https://speakerdeck.com/javiergs/ser431-lecture-04
 **/

#pragma once

#include <GL/glut.h>
#include "GameObject.h"
#include "Camera.h"
#include "Common.h"

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
int moveSpeed = 50;
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

int lightning_x = 0;
int lightning_z = 0;
bool showLightning = true;

//Draw 10 boxes for collecting to make guesses
int box1x = -38000, box1z = 37000;
int box2x = -38000, box2z = -37000;
int box3x = -20000, box3z = 20000;
int box4x = -10000, box4z = -30000;
int box5x = -5000, box5z = -10000;
int box6x = 25000, box6z = 15000;
int box7x = 30000, box7z = -20000;
int box8x = 37000, box8z = 37000;
int box9x = 10000, box9z = 1000;
int box10x = 37000, box10z = -37000;

bool box1 = true;
bool box2 = true;
bool box3 = true;
bool box4 = true;
bool box5 = true;
bool box6 = true;
bool box7 = true;
bool box8 = true;
bool box9 = true;
bool box10 = true;
int guesses = 0;

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

void generateLightningPos() {
	lightning_x = (rand() % 100000 - 50000);
	lightning_z = (rand() % 100000 - 50000);

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
		if(!isGameModeOn){
			camera_y -= 50;
		}
		
		break;
	case 'd': case 'D':
		if (!isGameModeOn) {
			camera_y += 50;
		}
		break;
	case 'l': case 'L':
		if (guesses > 0 && camera_x > -895 && camera_x < 1236 && camera_z > -16890 && camera_z < -14800 && doOnce == true) {
			guesses--;
			if (leftBox) {
				correctChoice = true;
				totalScore++;
				doOnce = false;
				raiseAndLower = true;
				getInBox = false;
			}
			else {
				correctChoice = false;
				raiseAndLower = true;
				getInBox = false;
			}
		}
		else {
			getInBox = true;
		}
		break;
	case 'r': case 'R':
		if (guesses > 0 &&camera_x > -895 && camera_x < 1236
			&& camera_z > -16890 && camera_z < -14800 && doOnce == true) {
			guesses--;
			if (!leftBox) {
				correctChoice = true;
				totalScore++;
				doOnce = false;
				raiseAndLower = true;
				getInBox = false;
			}

			else {
				correctChoice = false;
				raiseAndLower = true;
				getInBox = false;
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
		angle -= 0.05f;
		orientMe(angle);
		playerLook += 5;

		break;
	case GLUT_KEY_RIGHT:
		angle += 0.05f;
		orientMe(angle);
		playerLook -= 5;

		break;
	}

	glutPostRedisplay();
}