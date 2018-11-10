/**
 * SER 431
 * https://speakerdeck.com/javiergs/ser431-lecture-04
 **/

#pragma once

#include <math.h>
#include <GL/glut.h>
#include "GameObject.h"

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


float camera_viewing_x = 0.0;
float camera_viewing_y = 300.0;
float camera_viewing_z = 0.0;

float total_moving_angle = 0.0;

//Moving box values
GameObject jet = GameObject(
	0.0f, 100.0f, 0.0f,
	0.0f, 0.0f, 1.0f);

const int perlinMeshSize = 80000;

float meshSize = (perlinMeshSize - 1) / 2;
int cameraSpeed = 1;
int boxSpeed = 10;

float runway_x = -10;
float runway_y = 50;
float runway_z = -1000;

void updateBoxPositon(Mesh* mesh, int xOffset, int zOffset) {
	for (Vec3f i : mesh->dot_vertex) {
		float x = jet.getPosX();
		float z = jet.getPosZ();
		if (abs(x - (i.x - xOffset)) < 200 && abs(z - (i.z - zOffset)) < 200) {
			jet.setPosY(10 + i.y);
		}
	}
}

float moveBlock = 0.0;
float moveBlock_side = 0.0;
