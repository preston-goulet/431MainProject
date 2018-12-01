#pragma once

#include <stdlib.h>
#include <GL/glut.h>
#include <glut.h>
#include <fstream>
#include "mesh.h"
#include "texture.h"
#include "render.h"
#include "controls.h"
#include "Menu.h"
#include "particles.h"
#include "timer.h"
#include "GameObject.h"
#include "Curve.h"
#include "CurveFollower.h"
#include "nurbs_flag.h"
#include "ObjectPathing.h"


// global
Mesh *mesh1, *mesh2, *mesh3, *mesh4, *mesh5, *mesh6, *mesh7, *mesh8, *mesh9, *mesh11;
Mesh *mesh[10];
GLuint display1, display2, display3, display4, display5, display6, display7, displayBrick;
GLuint displayLists[10];

GLuint textures[15];
GLuint jetMesh, runway;
GLuint boundingBox;

const int boundaryMeshSize = 60000;
const int skyBoxMeshSize = 80000;

vector<Points> box_spawn;

float randY = 0;
float randX = 0;
float randZ = 0;

//Moving Flat
float px, py;//for arrow

// reshape
void reshape(int w, int h) {
	window_width = w;
	window_height = h;
	if (h == 0) h = 1;
	window_ratio = 1.0f * w / h;
}

// text
void renderBitmapString(float x, float y, float z, const char *string) {
	const char *c;
	glRasterPos3f(x, y, z);   // fonts position
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

// init
void init() {
	// particles
	init_frame_timer();

	// menu
	addMenu();

	// configuration
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	// mesh
	//heightMap = getHeightMap(mesh1);
	mesh1 = createPerlinPlane(80000, 80000, 400);
	mesh2 = createCube();
	mesh3 = createCube();
	mesh4 = createCube();
	mesh5 = createSkyBox(skyBoxMeshSize);
	mesh6 = createPlane(boundaryMeshSize, boundaryMeshSize, boundaryMeshSize/10);
	mesh7 = createCube();
	mesh8 = loadFile("OBJfiles/f-16.obj");
	mesh9 = createPlane(2000, 2000, 2000);
	mesh11 = createPlane(2000, 2000, 200);
	mesh[0] = createPlane(10, 10, 1);
	mesh[1] = createPlane(5, 5, 1);

	// normals
	calculateNormalPerFace(mesh1);
	calculateNormalPerFace(mesh2);
	calculateNormalPerFace(mesh3);
	calculateNormalPerFace(mesh4);
	calculateNormalPerFace(mesh5);
	calculateNormalPerFace(mesh6);
	calculateNormalPerFace(mesh7);
	calculateNormalPerFace(mesh8);
	calculateNormalPerFace(mesh9);
	calculateNormalPerFace(mesh11);
	calculateNormalPerFace(mesh[0]);
	calculateNormalPerFace(mesh[1]);
	calculateNormalPerVertex(mesh1);
	calculateNormalPerVertex(mesh2);
	calculateNormalPerVertex(mesh3);
	calculateNormalPerVertex(mesh4);
	calculateNormalPerVertex(mesh5);
	calculateNormalPerVertex(mesh6);
	calculateNormalPerVertex(mesh7);
	calculateNormalPerVertex(mesh8);
	calculateNormalPerVertex(mesh9);
	calculateNormalPerVertex(mesh11);
	calculateNormalPerVertex(mesh[0]);
	calculateNormalPerVertex(mesh[1]);

	//Bounding point calc
	calculateBoundingPoints(mesh8);
	
	// textures
	loadBMP_custom(textures, "_BMP_files/grass.bmp", 0);
	loadBMP_custom(textures, "_BMP_files/oldbox.bmp", 1);
	codedTexture(textures, 2, 0); //Sky texture - noise multiscale. Type=0
	codedTexture(textures, 3, 1); //Marble texture - noise marble. Type=1
	loadBMP_custom(textures, "_BMP_files/cubesky.bmp", 4);
	codedTexture(textures, 5, 2); //Fire texture - noise fire. Type=2
	codedTexture(textures, 6, 0); //Fire texture - noise fire. Type=2
	loadBMP_custom(textures, "_BMP_files/runway.bmp", 7);
	loadBMP_custom(textures, "_BMP_files/brick.bmp", 8);
	loadBMP_custom(textures, "_BMP_files/fireParticle.bmp", 9);
	
	// display lists
	display1 = meshToDisplayList(mesh1, 1, textures[0]);
	display2 = meshToDisplayList(mesh2, 2, textures[1]);
	display3 = meshToDisplayList(mesh3, 3, textures[2]);
	display4 = meshToDisplayList(mesh4, 4, textures[3]);
	display5 = meshToDisplayList(mesh5, 5, textures[4]);
	display6 = meshToDisplayList(mesh6, 6, textures[5]);//Lava
	display7 = meshToDisplayList(mesh7, 7, textures[6]);
	jetMesh = meshToDisplayListObjects(mesh8, 8);
	runway = meshToDisplayList(mesh9, 10, textures[7]);
	displayBrick = meshToDisplayList(mesh11, 11, textures[8]);
	displayLists[0] = meshToDisplayList(mesh[0], 12, textures[0]);
	displayLists[1] = meshToDisplayList(mesh[1], 13, textures[9]);

	boundingBox = boundingBoxToDisplayList(mesh8, 9);
	
	// light
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	GLfloat light_ambient[]  = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	// Fog
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	GLfloat fogColor[4] = { 0.5, 0.5, 0.9, 1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.75);
	glFogf(GL_FOG_START, 10000.0);
	glFogf(GL_FOG_END, 70000);

	glClearStencil(0);

	// floor vertex
	dot_vertex_floor.push_back(Vec3<GLfloat>(-2000.0, 0.0, 2000.0));
	dot_vertex_floor.push_back(Vec3<GLfloat>(2000.0, 0.0, 2000.0));
	dot_vertex_floor.push_back(Vec3<GLfloat>(2000.0, 0.0, -2000.0));
	dot_vertex_floor.push_back(Vec3<GLfloat>(-2000.0, 0.0, -2000.0));
	calculate_floor_normal(&floor_normal, dot_vertex_floor);
	//=========================================
	// Nurb Flab
	//=========================================
	//Original
	nurbsflag = gluNewNurbsRenderer();
	gluNurbsProperty(nurbsflag, GLU_SAMPLING_TOLERANCE, 100.0);

	//Shadow
	nurbsflag_three = gluNewNurbsRenderer();
	gluNurbsProperty(nurbsflag_three, GLU_SAMPLING_TOLERANCE, 100.0);

	gluNurbsProperty(nurbsflag_three, GLU_DISPLAY_MODE, GLU_FILL); //GLU_OUTLINE_POLYGON
	generateRandomNumber();

	//Particles
	leaves = ParticleSystem(displayLists[0], true, PARTICLE_LEAVES);
	rain = ParticleSystem(displayLists[1], true, PARTICLE_RAIN);
}

