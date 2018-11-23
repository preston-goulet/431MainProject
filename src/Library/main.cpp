/*
Lecture 25
http://slides.com/javiergs/ser332-l25
javiergs@asu.edu

Taylor Greeff
Preston Goulet
*/

#include <vector>
#include <imathvec.h>
#include <glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <wingdi.h>
#include "noise.h"
#include <stdlib.h> 
#include <GL\glut.h>
#include "GameObject.h"
#include "Render.h"

#define PI 3.1415926

using namespace std;
using namespace Imath;

GameObject skybox = GameObject();
Render render;

int width = 1200;
int height = 600;
float ratio = 1.0;

// controling parameters
int mouse_button;
int mouse_x = 0;
int mouse_y = 0;
float scale = 0.3;
float x_angle = 0.0;
float y_angle = 0.0;

// t_scale
float t_scale(float x) {
	// assuming noise report values -2.xxx to 2.xxx
	return (x + 2) / 4;
}

// marbleMap
Vec3f marbleMap(float a) {
	Vec3f black = Vec3f(0, 0, 0);
	Vec3f color = Vec3f(1, 1, 1);
	return ((1 - a) * color + a * black);
}

// skyMap
Vec3f skyMap(float a) {
	Vec3f white = Vec3f(1, 1, 1);
	Vec3f color = Vec3f(1, 0, 0);
	return ((1 - a) * white + a * color);
}

// Create texture from algorithm
void codedTexture(UINT textureArray[], int n, int type) {
	const int TexHeight = 128;
	const int TexWidth = 128;
	// create texture in memory
	GLubyte textureImage[TexHeight][TexWidth][3];
	ImprovedNoise noise;
	Vec3f pixelColor;
	for (int i = 0; i < TexHeight; i++)
		for (int j = 0; j < TexWidth; j++) {
			if (type == 0) pixelColor = skyMap(t_scale(noise.perlinMultiscale(i * 5, j * 5)));
			else pixelColor = marbleMap(t_scale(noise.perlinMarble(i * 5, j * 5)));
			textureImage[i][j][0] = pixelColor[0] * 255;
			textureImage[i][j][1] = pixelColor[1] * 255;
			textureImage[i][j][2] = pixelColor[2] * 255;
		}
	// setup texture
	glGenTextures(1, &textureArray[n]);
	glBindTexture(GL_TEXTURE_2D, textureArray[n]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // must set to 1 for compact data
										   // glTexImage2D Whith size and minification
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TexWidth, TexHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, textureImage); // BGRA to include alpha
}

// init
void init() {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	ratio = (double)width / (double)height;
	// mesh
	skybox.mesh.createSkybox(6000);
	skybox.texture.loadBMP_custom("_BMP_files/Cubeds.bmp");
	skybox.meshToDisplayList(0);

	// light
	GLfloat light_ambient[] = { 255.0, 255.0, 255.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

// main
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Textures");
	glutReshapeFunc(render.reshape);
	glutDisplayFunc(render.display);
	glutIdleFunc(render.idle);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	init();

	glutMainLoop();

	return 0;
}

