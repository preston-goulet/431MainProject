#pragma once

#include <stdlib.h>
#include <GL/glut.h>
#include <glut.h>
#include <fstream>
#include "Menu.h"
#include "mesh.h"
#include "texture.h"
#include "render.h"
#include "controls.h"

#include "particles.h"
#include "timer.h"
#include "GameObject.h"
#include "Curve.h"
#include "CurveFollower.h"
#include "nurbs_flag.h"
#include "ObjectPathing.h"

#define random_number() (((float) rand())/((float) RAND_MAX))
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

// Use in thunderbolt
class Line {
public:
    float x1, y1, x2, y2, z1,z2;
    Line(float a, float b, float c, float d, float e, float f) {
        x1 = a; y1 = b; z1 = c;  x2 = d; y2 = e; z2 = f;
    }
};
vector<Line> thunderbolt;

// calculate middle points with a displacement for the thunderbolt
Vec3f calculateMiddle(Vec3f p1, Vec3f p2, int level) {
    Vec3f O = (p2+p1)/2;
    float m1 = (p2.y - p1.y) / (p2.x - p1.x);
    int signo = (random_number()>0.5) ? 1 : -1;
    float x = O.x + signo*((cos(PI/2 + atan(m1)))/ pow(2, level)); // higher level, small displacement
    float y = O.y + signo*((sin(PI/2 + atan(m1)))/ pow(2, level));
    return Vec3f(x, y, signo*random_number() / pow(2, level));
}

// create thunderbolt as fractal
void createBolt(Vec3f p1, Vec3f p2, int level) {
    thunderbolt.push_back(Line(p1.x,p1.y,p1.z, p2.x,p2.y, p2.z));
    // printf("* \t INSERT-A (%f, %f, %f) -- (%f, %f, %f)\n\n", p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
    for (int t = 0; t < level; t++) {
        int tam = thunderbolt.size();
        Vec3f middle; int i;
        for (i = 0; i < tam; i++) {
            p1.x = thunderbolt[0].x1; p1.y = thunderbolt[0].y1; p1.z = thunderbolt[0].z1;
            p2.x = thunderbolt[0].x2; p2.y = thunderbolt[0].y2; p2.z = thunderbolt[0].z2;
            thunderbolt.erase(thunderbolt.begin());
            // printf("%i \t DELETE-- (%f, %f) -- (%f, %f)\n", i,p1.x, p1.y, p2.x, p2.y);
            middle = calculateMiddle(p1, p2, t);
            thunderbolt.push_back(Line(p1.x, p1.y, p1.z, middle.x,middle.y, middle.z));
            thunderbolt.push_back(Line(middle.x, middle.y, middle.z, p2.x,p2.y, p2.z));
            // printf("%i \t Middle level %d)\n", i, t);
            // printf("%i \t INSERT-A (%f, %f, %f) -- (%f, %f, %f)\n", i, p1.x, p1.y, p1.z, middle.x, middle.y, middle.z);
            // printf("%i \t INSERT-B (%f, %f, %f) -- (%f, %f, %f)\n", i, middle.x, middle.y, middle.z, p2.x, p2.y, p2.z);
        }
        // extension line
        Vec3f direction = middle - p1;
        Vec3f pin = middle + direction * 0.7;
        thunderbolt.push_back(Line(middle.x, middle.y, middle.z, pin.x, pin.y, pin.z));
        //printf("%i \t INSERT-C (%f, %f, %f) -- (%f, %f, %f)\n\n", i, middle.x, middle.y, middle.z, pin.x, pin.y, pin.z);
    }
    
}

// init
void init() {
	// particles
	init_frame_timer();
    
    // init thunderbolt
    srand(time(0));
    createBolt(Vec3f(5, 5, 0), Vec3f(0, 0, 0), 4);
    
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
	mesh[2] = createPlane(80000, 80000, 400);

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
	calculateNormalPerFace(mesh[2]);
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
	calculateNormalPerVertex(mesh[2]);

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
	displayLists[2] = meshToDisplayList(mesh[2], 14, textures[0]);

	boundingBox = boundingBoxToDisplayList(mesh8, 9);
	
	// light
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	GLfloat mat_diffuse[] = { 0.75f, 0.75f, 0.75f, 1. };
	GLfloat mat_specular[] = { 0.75f, 0.75f, 0.75f, 1. };
	GLfloat mat_ambient[] = { 0.75f, 0.75f, 0.75f, 1. };
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60, 1.0, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0., 0., -6.);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
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
	gluNurbsProperty(nurbsflag, GLU_SAMPLING_TOLERANCE, 25.0);

	//Shadow
	nurbsflag_three = gluNewNurbsRenderer();
	gluNurbsProperty(nurbsflag_three, GLU_SAMPLING_TOLERANCE, 100.0);

	gluNurbsProperty(nurbsflag_three, GLU_DISPLAY_MODE, GLU_FILL); //GLU_OUTLINE_POLYGON
	generateRandomNumber();

	//Particles
	leaves = ParticleSystem(displayLists[0], true, PARTICLE_LEAVES);
	rain = ParticleSystem(displayLists[1], true, PARTICLE_RAIN);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
}

