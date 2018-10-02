/**
 * SER 431
 * https://speakerdeck.com/javiergs/ser431-lecture-04
 **/

#include <stdlib.h>
#include <GL/glut.h>
#include <glut.h>
#include <fstream>
#include "mesh.h"
#include "texture.h"
#include "render.h"
#include "controls.h"


// global
Mesh *mesh1, *mesh2, *mesh3, *mesh4, *mesh5, *mesh6, *mesh7;
GLuint display1, display2, display3, display4, display5, display6, display7;
GLuint textures[5];

const int boundaryMeshSize = 60000;
const int skyBoxMeshSize = 80000;


// init
void init() {

	// configuration
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	// mesh
	mesh1 = createPerlinPlane(80000, 80000, 400);
	//heightMap = getHeightMap(mesh1);
	mesh2 = createCube();
	mesh3 = createCube();
	mesh4 = createCube();
	mesh5 = createSkyBox(skyBoxMeshSize);
	mesh6 = createPlane(boundaryMeshSize, boundaryMeshSize, boundaryMeshSize/10);
	mesh7 = createCube();
	
	// normals
	calculateNormalPerFace(mesh1);
	calculateNormalPerFace(mesh2);
	calculateNormalPerFace(mesh3);
	calculateNormalPerFace(mesh4);
	calculateNormalPerFace(mesh5);
	calculateNormalPerFace(mesh6);
	calculateNormalPerFace(mesh7);
	calculateNormalPerVertex(mesh1);
	calculateNormalPerVertex(mesh2);
	calculateNormalPerVertex(mesh3);
	calculateNormalPerVertex(mesh4);
	calculateNormalPerVertex(mesh5);
	calculateNormalPerVertex(mesh6);
	calculateNormalPerVertex(mesh7);
	
	// textures
	loadBMP_custom(textures, "_BMP_files/grass.bmp", 0);
	loadBMP_custom(textures, "_BMP_files/oldbox.bmp", 1);
	codedTexture(textures, 2, 0); //Sky texture - noise multiscale. Type=0
	codedTexture(textures, 3, 1); //Marble texture - noise marble. Type=1
	loadBMP_custom(textures, "_BMP_files/cubesky.bmp", 4);
	codedTexture(textures, 5, 2); //Fire texture - noise fire. Type=2
	codedTexture(textures, 6, 0); //Fire texture - noise fire. Type=2

	// display lists
	display1 = meshToDisplayList(mesh1, 1, textures[0]);
	display2 = meshToDisplayList(mesh2, 2, textures[1]);
	display3 = meshToDisplayList(mesh3, 3, textures[2]);
	display4 = meshToDisplayList(mesh4, 4, textures[3]);
	display5 = meshToDisplayList(mesh5, 5, textures[4]);
	display6 = meshToDisplayList(mesh6, 6, textures[5]);//Lava
	display7 = meshToDisplayList(mesh7, 7, textures[6]);


	
	// light
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	GLfloat light_ambient[]  = { 255.0, 255.0, 255.0, 1.0 };
	GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	//===================================
	// Fog
	//===================================
	glEnable(GL_FOG);
	
	glFogi(GL_FOG_MODE, GL_LINEAR);
	GLfloat fogColor[4] = { 0.5, 0.5, 0.9, 1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.75);
	glFogf(GL_FOG_START, 10000.0);
	glFogf(GL_FOG_END, 70000);

	glClearStencil(0);
}

// reshape
void reshape(int w, int h) {
	window_width = w;
	window_height = h;
}

// text
void renderBitmapString(float x, float y, float z, const char *string) {
	const char *c;
	glRasterPos3f(x, y, z);   // fonts position
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

// display
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, window_width, window_height);
	gluPerspective(45, window_ratio, 10, 100000);
	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// lookAt
	gluLookAt(camera_x, camera_y, camera_z,
		camera_viewing_x, camera_viewing_y, camera_viewing_z,
		0.0f, 1.0f, 0.0f);

	// camera
	glRotatef(camera_rotate, 0.0f, 1.0f, 0.0f);
	//glTranslatef(camera_x, 0, camera_z);

	// box 1
	//glPushMatrix();
	//glTranslatef(moveBlock, 200, moveBlock_side);
	//glCallList(display2);
	//glPopMatrix();
	//// box 2
	//glPushMatrix();
	//glTranslatef(200, 200, 0);
	//glCallList(display3);
	//glPopMatrix();
	//// box 3
	//glPushMatrix();
	//glTranslatef(-200, 200, 0);
	//glCallList(display4);
	//glPopMatrix();

	// skybox
	glPushMatrix();
	glTranslatef(-skyBoxMeshSize / 2, -skyBoxMeshSize / 2, -skyBoxMeshSize / 2);
	glCallList(display5);
	glPopMatrix();

	// ===== STENCIL DRAW ============================================================================

	glEnable(GL_STENCIL_TEST); //Start using the stencil
	glDisable(GL_DEPTH_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //Disable writing colors in frame buffer
	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF); //Place a 1 where rendered
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); 	//Replace where rendered
	// PLAIN for the stencil
	glPushMatrix();
		glTranslatef(-boundaryMeshSize / 2, -50, -boundaryMeshSize / 2);
		glCallList(display6);
	glPopMatrix();
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //Reenable color
	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Keep the pixel


	//Box Reflection
	glPushMatrix();	
	glScalef(1.0, -1.0, 1.0);
	glTranslatef(boxPositionX, 50, boxPositionZ);
	glRotatef(boxRotationX, 1.0, 0.0, 0.0);
	//glRotatef(total_moving_angle, 0.0, 1.0, 0.0);
	//glRotatef(boxRotationZ, 0.0, 0.0, 1.0);
	glCallList(display7);
	glPopMatrix();

	// STENCIL-STEP 4. disable it
	glDisable(GL_STENCIL_TEST);

	//========Regular 3d environment=====================================
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.7, 0.0, 0.0, 0.3);
	glColor4f(1.0, 1.0, 1.0, 0.3);

	//Water
	glPushMatrix();
	glTranslatef(-boundaryMeshSize / 2, -50, -boundaryMeshSize / 2);
	glCallList(display6);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	//moving box
	glPushMatrix();
	glTranslatef(boxPositionX, boxPositionY, boxPositionZ);
	glRotatef(boxRotationX, 1.0, 0.0, 0.0);
	glRotatef(total_moving_angle, 0.0, 1.0, 0.0);
	glRotatef(boxRotationZ, 0.0, 0.0, 1.0);
	glCallList(display7);
	glPopMatrix();

	//plane
	glPushMatrix();
	glTranslatef(-perlinMeshSize / 2, 0, -perlinMeshSize / 2);
	glCallList(display1);
	glPopMatrix();

	//end
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// texto
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(0.0, window_height - 13.0f, 0.0f, "Use [Arrows] to move in plain");
	renderBitmapString(0.0, window_height - 26.0f, 0.0f, "Use [W and S] to look up and down");
	//renderBitmapString(0.0, window_height - 39.0f, 0.0f, "Use I, J, K and L to move the box");
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glutSwapBuffers();
}

//// rotate what the user see
//void rotate_point(float angle) {
//	float s = sin(angle);
//	float c = cos(angle);
//	// translate point back to origin:
//	camera_viewing_x -= camera_x;
//	camera_viewing_z -= camera_z;
//	// rotate point
//	float xnew = camera_viewing_x * c - camera_viewing_z * s;
//	float znew = camera_viewing_x * s + camera_viewing_z * c;
//	// translate point back:
//	camera_viewing_x = xnew + camera_x;
//	camera_viewing_z = znew + camera_z;
//}

// callback function for keyboard (alfanumeric keys)
void callbackKeyboard(unsigned char key, int x, int y) {

	switch (key) {
		case 'w': case 'W':
			camera_viewing_y += (10);
			break;
		case 's': case 'S':
			camera_viewing_y -= 10;
			break;
		case 'a': case 'A':
			camera_rotate -= (5); //rotates camera
			break;
		case 'd': case 'D':
			camera_rotate += (5);
			break;
		case 'l': case 'L':
			moveBlock += (10);
			break;
		case 'j': case 'J':
			moveBlock -= (10);
			break;
		case 'k': case 'K':
			if (boxPositionX <= meshSize && boxPositionX >= -meshSize) {
				boxPositionX += (-boxSpeed) * sin(total_moving_angle);
			}
			if (boxPositionZ <= meshSize && boxPositionZ >= -meshSize) {
				boxPositionZ += (-boxSpeed) * -cos(total_moving_angle);
			}
			break;
		case 'i': case 'I':
			if (boxPositionX <= meshSize && boxPositionX >= -meshSize) {
				boxPositionX += (boxSpeed)* sin(total_moving_angle);
			}
			if (boxPositionZ <= meshSize && boxPositionZ >= -meshSize) {
				boxPositionZ += (boxSpeed) * -cos(total_moving_angle);
			}
			break;
	}

	if (boxPositionX > meshSize) {
		boxPositionX = meshSize;
	}
	else if (boxPositionX < -meshSize) {
		boxPositionX = -meshSize;
	}

	// box Z verification
	if (boxPositionZ > meshSize) {
		boxPositionZ = meshSize;
	}
	else if (boxPositionZ < -meshSize) {
		boxPositionZ = -meshSize;
	}
}

// callback function for arrows
void specialkeys(int key, int x, int y) {
	
	if (key == GLUT_KEY_LEFT) {
		camera_x -= 50;
		camera_viewing_x -= 50;
		boxPositionX -= 50;
	} else if (key == GLUT_KEY_RIGHT) {
		camera_x += 50;
		camera_viewing_x += 50;
		boxPositionX += 50;
	} else if (key == GLUT_KEY_DOWN) {
		camera_z += 50;
		camera_viewing_z += 50;
		boxPositionZ += 50;
	} else if (key == GLUT_KEY_UP) {
		camera_z -= 50;
		camera_viewing_z -= 50;
		boxPositionZ -= 50;
	}

	// Camera X verification
	if (camera_x >  meshSize) {
		camera_x = meshSize;
		camera_viewing_x = meshSize;
	}
	else if (camera_x < -meshSize) {
		camera_x = -meshSize;
		camera_viewing_x = -meshSize;
	}

	// Camera Z verification
	if (camera_z >meshSize) {
		camera_z = meshSize;
		camera_viewing_z = meshSize;
	}
	else if (camera_z < -meshSize) {
		camera_z = -meshSize;
		camera_viewing_z = -meshSize;
	}

	// box X verification
	if (boxPositionX >meshSize) {
		boxPositionX = meshSize;
	}
	else if (boxPositionX < -meshSize) {
		boxPositionX = -meshSize;
	}

	// box Z verification
	if (boxPositionZ >meshSize) {
		boxPositionZ = meshSize;
	}
	else if (boxPositionZ < -meshSize) {
		boxPositionZ = -meshSize;
	}
}

// main
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("The Game");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	
	glutSpecialFunc(specialkeys);
	glutKeyboardFunc(callbackKeyboard);
	init();
	glutMainLoop();
	return 0;
}
