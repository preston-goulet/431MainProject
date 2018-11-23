/**
* SER 431
* Built upon from https://speakerdeck.com/javiergs/ser431-lecture-04
**/

#pragma once
#include <GL/glut.h>
#include "mesh.h"

#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_WRAP_R 0x8072

int width = 800, height = 800;
const float ratio = 1.0;

class Render {
private:
	bool isReflectionOn = true;
	bool isFogOn = false;
	bool areShadowsOn = false;

	void toggleReflection();
	void menuListener(int option);

	void textToDisplay();

public:
	GLfloat light_position[4];
	GLfloat shadow_matrix[4][4];
	Vec3f floor_normal;
	vector<Vec3f> dot_vertex_floor;

	float lightAngle = 0.0, lightHeight = 100;

	void calculate_floor_normal(Vec3f * plane, vector<Vec3f> dot_floor);
	void shadowMatrix(GLfloat shadowMat[4][4], Vec3f plane_normal, GLfloat lightpos[4]);
	void drawLightArrow();
	GLuint meshToDisplayList(Mesh * m, int id, int texture);
	void addMenu();
	void addText(string text, int xPos);
	void renderBitmapString(float x, float y, float z, const char * string);
	void reshape(int w, int h);
	void display(void);
	void idle(void);
};

void toggleReflection() {
	if (isReflectionOn) {
		isReflectionOn = false;
	}
	else {
		isReflectionOn = true;
	}
}

void menuListener(int option) {
	switch (option) {
	case 0:
		toggleReflection();
		break;
	}

	glutPostRedisplay();
}