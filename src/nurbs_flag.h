#pragma once

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define V_NUMPOINTS    4
#define U_NUMPOINTS	   4
#define V_NUMKNOTS    (V_NUMPOINTS + 4)
#define U_NUMKNOTS    (U_NUMPOINTS + 4)

// Knot sequences for cubic bezier surface and trims 
GLfloat sknots[V_NUMKNOTS] = { 0., 0., 0., 0., 1., 1., 1., 1. };
GLfloat tknots[U_NUMKNOTS] = { 0., 0., 0., 0., 1., 1., 1., 1. };


// Control points for the flag. The Z values are modified to make it wave
GLfloat ctlpoints[V_NUMPOINTS][U_NUMPOINTS][3] = {
	{ { 0., 3., 0. },{ 1., 3., 0. },{ 2., 3., 0 },{ 3., 3., 0. } },
	{ { 0., 2., 0. },{ 1., 2., 0. },{ 2., 2., 0 },{ 3., 2., 0. } },
	{ { 0., 1., 0. },{ 1., 1., 0. },{ 2., 1., 0 },{ 3., 1., 0. } },
	{ { 0., 0., 0. },{ 1., 0., 0. },{ 2., 0., 0 },{ 3., 0., 0. } }
};


GLUnurbsObj *nurbsflag;          //original
GLUnurbsObj *nurbsflag_two;      //reflection
GLUnurbsObj *nurbsflag_three;    //shadow


// draw_control_graph
void draw_control_graph(GLfloat cpoints[V_NUMPOINTS][U_NUMPOINTS][3]) {
	int s, t;
	glDisable(GL_LIGHTING);
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	for (s = 0; s < V_NUMPOINTS; s++)
		for (t = 0; t < U_NUMPOINTS - 1; t++) {
			glVertex3fv(cpoints[s][t]);
			glVertex3fv(cpoints[s][t + 1]);
		}
	for (t = 0; t < U_NUMPOINTS; t++)
		for (s = 0; s < V_NUMPOINTS - 1; s++) {
			glVertex3fv(cpoints[s][t]);
			glVertex3fv(cpoints[s + 1][t]);
		}
	glEnd();
	glEnable(GL_LIGHTING);
}


// draw_nurb
void draw_nurb() {
	static GLfloat angle = 0.0;
	int i, j;
	// wave the flag by rotating Z coords though a sine wave
	for (i = 1; i < 4; i++)
		for (j = 0; j < 4; j++)
			ctlpoints[i][j][2] = sin((GLfloat)i + angle);
	angle += 0.01;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	
	glRotatef(90, 0., 0., 1.);
	gluBeginSurface(nurbsflag);
	gluNurbsSurface(nurbsflag, V_NUMKNOTS, sknots, U_NUMKNOTS, tknots, 3 * U_NUMPOINTS, 3, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurbsflag);

	draw_control_graph(ctlpoints);
	glPopMatrix();
}

// draw_nurb
void draw_nurb2() {
	static GLfloat angle = 0.0;
	int i, j;
	// wave the flag by rotating Z coords though a sine wave
	for (i = 1; i < 4; i++)
		for (j = 0; j < 4; j++)
			ctlpoints[i][j][2] = sin((GLfloat)i + angle);
	angle += 0.01;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	glRotatef(90, 0., 0., 1.);
	gluBeginSurface(nurbsflag_two);
	gluNurbsSurface(nurbsflag_two, V_NUMKNOTS, sknots, U_NUMKNOTS, tknots, 3 * U_NUMPOINTS, 3, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurbsflag);

	draw_control_graph(ctlpoints);
	glPopMatrix();
}

// draw_nurb
void draw_nurb3() {
	static GLfloat angle = 0.0;
	int i, j;
	// wave the flag by rotating Z coords though a sine wave
	for (i = 1; i < 4; i++)
		for (j = 0; j < 4; j++)
			ctlpoints[i][j][2] = sin((GLfloat)i + angle);
	angle += 0.01;
	glPushMatrix();

	glRotatef(90, 0., 0., 1.);
	gluBeginSurface(nurbsflag_three);
	gluNurbsSurface(nurbsflag_three, V_NUMKNOTS, sknots, U_NUMKNOTS, tknots, 3 * U_NUMPOINTS, 3, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurbsflag_three);

	draw_control_graph(ctlpoints);
	glPopMatrix();
}