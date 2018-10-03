/**
 * SER 431
 * https://speakerdeck.com/javiergs/ser431-lecture-04
 **/

#pragma once
#include <GL/glut.h>
#include "mesh.h"

#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_WRAP_R 0x8072

bool isReflectionOn = true;
bool isFogOn = false;

GLfloat light_position[4];
GLfloat shadow_matrix[4][4];
Vec3f floor_normal;
vector<Vec3f> dot_vertex_floor;
float lightAngle = 0.0, lightHeight = 100;
int renderShadow = 1;

// calculate floor normal
void calculate_floor_normal(Vec3f *plane, vector<Vec3f> dot_floor) {
	Vec3<GLfloat> AB = dot_floor[1] - dot_floor[0];
	Vec3<GLfloat> AC = dot_floor[2] - dot_floor[0];
	*plane = AB.cross(AC);
}

// Create a matrix that will project the desired shadow
void shadowMatrix(GLfloat shadowMat[4][4], Vec3f plane_normal, GLfloat lightpos[4]) {
	GLfloat dot;
	Vec3f lightpos_v; lightpos_v.x = lightpos[0]; lightpos_v.y = lightpos[1]; lightpos_v.z = lightpos[2];
	dot = plane_normal.dot(lightpos_v);
	shadowMat[0][0] = dot - lightpos[0] * plane_normal[0];
	shadowMat[1][0] = 0.f - lightpos[0] * plane_normal[1];
	shadowMat[2][0] = 0.f - lightpos[0] * plane_normal[2];
	shadowMat[3][0] = 0.f - lightpos[0] * plane_normal[3];
	shadowMat[0][1] = 0.f - lightpos[1] * plane_normal[0];
	shadowMat[1][1] = dot - lightpos[1] * plane_normal[1];
	shadowMat[2][1] = 0.f - lightpos[1] * plane_normal[2];
	shadowMat[3][1] = 0.f - lightpos[1] * plane_normal[3];
	shadowMat[0][2] = 0.f - lightpos[2] * plane_normal[0];
	shadowMat[1][2] = 0.f - lightpos[2] * plane_normal[1];
	shadowMat[2][2] = dot - lightpos[2] * plane_normal[2];
	shadowMat[3][2] = 0.f - lightpos[2] * plane_normal[3];
	shadowMat[0][3] = 0.f - lightpos[3] * plane_normal[0];
	shadowMat[1][3] = 0.f - lightpos[3] * plane_normal[1];
	shadowMat[2][3] = 0.f - lightpos[3] * plane_normal[2];
	shadowMat[3][3] = dot - lightpos[3] * plane_normal[3];
}

// draw an arrow to visualize the source of light
void drawLightArrow() {
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 0.0);
	// draw arrowhead. 
	glTranslatef(light_position[0], light_position[1], light_position[2]);
	glRotatef(lightAngle * -180.0 / 3.141592, 0, 1, 0);
	//glRotatef(atan(light_position[1] / 500) * 180.0 / 3.141592, 0, 0, 1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	glVertex3f(20, 10, 10);
	glVertex3f(20, -10, 10);
	glVertex3f(20, -10, -10);
	glVertex3f(20, 10, -10);
	glVertex3f(20, 10, 10);
	glEnd();
	// draw a  line from light direction
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(50, 0, 0);
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

// draw
GLuint meshToDisplayList(Mesh* m, int id, int texture) {
	GLuint listID = glGenLists(id);
	glNewList(listID, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture);
	//}
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < m->face_index_vertex.size(); i++) {
		// PER VERTEX NORMALS
		if ((!m->dot_normalPerVertex.empty() && !m->face_index_normalPerVertex.empty())) {
			glNormal3fv(&m->dot_normalPerVertex[m->face_index_normalPerVertex[i]].x);
		}
		// TEXTURES
		if (!m->dot_texture.empty() && !m->face_index_texture.empty()) {
			glTexCoord2fv(&m->dot_texture[m->face_index_texture[i]].x);
		}
		// COLOR
		Vec3f offset = (m->dot_vertex[m->face_index_vertex[i]]);
		// VERTEX
		glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		glVertex3fv(&m->dot_vertex[m->face_index_vertex[i]].x);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEndList();
	return listID;
}

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

void addMenu() {
	int optionsMenu = glutCreateMenu(menuListener);

	//add entries to submenu Colores
	glutAddMenuEntry("Toggle Reflection", 0);
	// create main menu
	int menu = glutCreateMenu(menuListener);
	glutAddSubMenu("Options", optionsMenu);

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}