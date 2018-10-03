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
#include "particles.h"
#include "timer.h"


// global
Mesh *mesh1, *mesh2, *mesh3, *mesh4, *mesh5, *mesh6, *mesh7, *mesh8;
GLuint display1, display2, display3, display4, display5, display6, display7;
GLuint textures[5];
GLuint jetMesh;
GLuint boundingBox;

vector<Points> box_spawn;

const int boundaryMeshSize = 60000;
const int skyBoxMeshSize = 80000;

bool doOnce = true;
float randY = 0;
float randX = 0;
float randZ = 0;


//Moving Flat
//float camera_x = 0.0f, camera_y = 20.0f, camera_z = 5.0f;
float lookx = 0.0f, looky = 0.0f, lookz = -1.0f;
float px, py;//for arrow
int moveSpeed = 50;
float angle = 0;
float playerLook = 0;


// draw particles
void drawParticles() {
	Particle* curr = ps.particle_head;
	// glPointSize(2);
	// glBegin(GL_POINTS);
	// while (curr) {
	//   glVertex3fv(curr->position);
	//	 curr = curr->next;
	// }
	// glEnd();
	while (curr) {
		glPushMatrix();
		glScalef(100.0, 100.0, 100.0);
		glTranslatef(curr->position[0], curr->position[1], curr->position[2]);
		glScalef(0.2, 0.2, 0.2);
		glColor4f(curr->color[0], curr->color[1], curr->color[2], 0.3);
		//front
		glBegin(GL_POLYGON); // fill connected polygon
		glVertex3f(0, 0, 0); // vertices of the triangle 1
		glVertex3f(0, 1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, 1, 0);
		glEnd();
		//back
		glBegin(GL_POLYGON); // fill connected polygon
		glVertex3f(0, 0, -1); // vertices of the triangle 1
		glVertex3f(0, 1, -1);
		glVertex3f(1, 1, -1);
		glVertex3f(1, 0, -1);
		glEnd();
		//left
		glBegin(GL_POLYGON); // fill connected polygon
		glVertex3f(0, 0, -1); // vertices of the triangle 1
		glVertex3f(0, 1, -1);
		glVertex3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glEnd();
		//right
		glBegin(GL_POLYGON); // fill connected polygon
		glVertex3f(1, 0, -1); // vertices of the triangle 1
		glVertex3f(1, 0, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, 1, -1);
		glEnd();
		//top
		glBegin(GL_POLYGON); // fill connected polygon
		glVertex3f(1, 1, -1); // vertices of the triangle 1
		glVertex3f(1, 1, 0);
		glVertex3f(0, 1, 0);
		glVertex3f(0, 1, -1);
		glEnd();
		//bottom
		glBegin(GL_POLYGON); // fill connected polygon
		glVertex3f(1, 0, -1); // vertices of the triangle 1
		glVertex3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, -1);
		glEnd();

		glPopMatrix();
		curr = curr->next;
	}

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

	addMenu();

	// mesh
	mesh1 = createPerlinPlane(80000, 80000, 400);
	//heightMap = getHeightMap(mesh1);
	mesh2 = createCube();
	mesh3 = createCube();
	mesh4 = createCube();
	mesh5 = createSkyBox(skyBoxMeshSize);
	mesh6 = createPlane(boundaryMeshSize, boundaryMeshSize, boundaryMeshSize/10);
	mesh7 = createCube();
	mesh8 = loadFile("OBJfiles/f-16.obj");
	
	// normals
	calculateNormalPerFace(mesh1);
	calculateNormalPerFace(mesh2);
	calculateNormalPerFace(mesh3);
	calculateNormalPerFace(mesh4);
	calculateNormalPerFace(mesh5);
	calculateNormalPerFace(mesh6);
	calculateNormalPerFace(mesh7);
	calculateNormalPerFace(mesh8);
	calculateNormalPerVertex(mesh1);
	calculateNormalPerVertex(mesh2);
	calculateNormalPerVertex(mesh3);
	calculateNormalPerVertex(mesh4);
	calculateNormalPerVertex(mesh5);
	calculateNormalPerVertex(mesh6);
	calculateNormalPerVertex(mesh7);
	calculateNormalPerVertex(mesh8);

	calculateBoundingPoints(mesh8);
	
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
	jetMesh = meshToDisplayListObjects(mesh8, 8);

	boundingBox = boundingBoxToDisplayList(mesh8, 9);
	
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
	if (isFogOn) {
		glEnable(GL_FOG);
	}
	
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

	//===========================================================================
	//  Camera
	//===========================================================================
	gluLookAt(camera_x, camera_y, camera_z,
		camera_x + lookx, camera_y + looky, camera_z + lookz,
		0.0f, 1.0f, 0.0f);

	// camera
	glPushMatrix();
		glScalef(scale, scale, scale);
		glTranslatef(0.0f, 0.0f, 0.0f);
	glPopMatrix();


	//=======================================
	// Box Targets
	//=======================================
	//box_spawn.push_back(Points());
	if (doOnce == true) {
		for (int i = 0; i < 3000; i++) {
			box_spawn.push_back(Points());
			randX = rand() % 5000 + (-5000);
			randY = rand() % 1000 + 200;
			randZ = rand() % 5000 + (-5000);

			box_spawn[i].x = randX;
			box_spawn[i].y = randY;
			box_spawn[i].z = randZ;
		}
	}

	// box 1
	glPushMatrix();
		for (int i = 0; i < 1000; i++) {
			glTranslatef( box_spawn[i].x, box_spawn[i].y, box_spawn[i].z );
			glCallList(display2);
		}
	glPopMatrix();

	//Box 2
	glPushMatrix();
		for (int i = 100; i < 2000; i++) {
			glTranslatef(box_spawn[i].x, box_spawn[i].y, box_spawn[i].z);
			glCallList(display3);
		}
	glPopMatrix();

	// Box 3
	glPushMatrix();
		for (int i = 200; i < 3000; i++) {
			glTranslatef(box_spawn[i].x, box_spawn[i].y, box_spawn[i].z);
			glCallList(display4);
		}
	glPopMatrix();
	doOnce = false;

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
	if (isReflectionOn) { //Add Toggle to reflection
		glPushMatrix();
		glTranslatef(-boundaryMeshSize / 2, -50, -boundaryMeshSize / 2);
		glCallList(display6); //Water
		glPopMatrix();
	}

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //Reenable color
	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Keep the pixel

	//Jet Reflection
	glPushMatrix();	
		glScalef(1.0, -1.0, 1.0);
		glTranslatef(camera_x, 14, camera_z - 500);
		glRotatef(180 + jet_rotate, 0.0, 1.0, 0.0);
		glTranslatef(lookx, looky, lookz);
		glScalef(10, 10, 10);
		glCallList(jetMesh); //display7
	glPopMatrix();

	//============================================
	//	Exhaust flames reflection
	//============================================
	if (areParticlesOn) {
		//Particles with box 1
		for (int i = 0; i < 50; i++) {
			ps.add();
		}
		ps2.add();
	}
	float time = calculate_frame_time();
	ps.update(time);
	ps2.update(time);
	for (int i = 0; i < 50; i++) {
		ps.remove();
	}
	ps.remove();
	glPushMatrix();
	glTranslatef(camera_x + 10, 5, camera_z - 400);
	glRotatef(90, 1, 0, 0);
	glRotatef(jet_rotate, 0.0, 1.0, 0.0);
	glScalef(.1, .1, .1);
	drawParticles();//flames
	glPopMatrix();


	// STENCIL-STEP 4. disable it
	glDisable(GL_STENCIL_TEST);

	//========Regular 3d environment=====================================
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.7, 0.0, 0.0, 0.9);
	glColor4f(1.0, 1.0, 1.0, 0.9);

	//Water
	glPushMatrix();
	glTranslatef(-boundaryMeshSize / 2, -50, -boundaryMeshSize / 2);
	glCallList(display6);
	glPopMatrix();

	//============================================
	//	Exhaust flames
	//============================================
	ps.remove();
	glPushMatrix();
	glTranslatef(camera_x + 12, camera_y - 60, camera_z - 450);
	glRotatef(90, 1, 0, 0);
	glRotatef(jet_rotate, 0.0, 1.0, 0.0);
	glScalef(.1, .1, .1);
	drawParticles();//flames
	glPopMatrix();
	
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHT0);

	if (areBoundingBoxesOn) {
		//bounding box
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 0.0);
		glTranslatef(camera_x, camera_y - 100, camera_z - 500);
		glRotatef(180 + jet_rotate, 0.0, 1.0, 0.0);
		glTranslatef(lookx, looky, lookz);
		glScalef(10, 10, 10);
		glCallList(boundingBox);

		glEnable(GL_LIGHTING);
		glPopMatrix();
	}

	//==========================
	// jet
	//==========================
	glPushMatrix();
		glTranslatef( camera_x, camera_y - 100, camera_z - 500 );
		glRotatef(180 + jet_rotate, 0.0, 1.0, 0.0);
		glTranslatef(lookx, looky, lookz);
		glScalef(10, 10, 10);
		glCallList(jetMesh); 
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

	//=======================================================================
	// Lighting disabled from here on to allow other colors on the screen
	//=======================================================================

	glDisable(GL_LIGHTING);

	// texto
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0.0, 1.0, 0.0);
	renderBitmapString(0.0, window_height - 13.0f, 0.0f, "Use [Arrows] to move in plain");
	renderBitmapString(0.0, window_height - 26.0f, 0.0f, "Use [W and S] to look up and down");
	//renderBitmapString(0.0, window_height - 39.0f, 0.0f, "Use I, J, K and L to move the box");
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	

	//=====================================
	// Second Viewport
	//=====================================
	glViewport(0, 0, window_width / 4, window_height / 4);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Draw box for hud
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 1200.0, 0.0, 250.0);
	
	glColor3f(.200, .200, .200);
	//Bottom tunnel
	glShadeModel(GL_FLAT);
	glBegin(GL_QUADS);
		glVertex2f( 0, 0 );//bottom left
		glVertex2f( window_width, 0 );//bottom right
		glVertex2f( window_width, window_height ); //top right
		glVertex2f( 0, window_height );//top left
	glEnd();

	
	glPopMatrix();
	glEnable(GL_LIGHTING);
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
			looky += .1;
			break;
		case 's': case 'S':
			looky -= .1;
			break;
		case 'a': case 'A':
			break;
		case 'd': case 'D':
			break;
	}

	if (jetPositionX > meshSize) {
		jetPositionX = meshSize;
	}
	else if (jetPositionX < -meshSize) {
		jetPositionX = -meshSize;
	}

	// box Z verification
	if (jetPositionZ > meshSize) {
		jetPositionZ = meshSize;
	}
	else if (jetPositionZ < -meshSize) {
		jetPositionZ = -meshSize;
	}
}

// callback function for arrows
void moveMeFlat(int i) {
	camera_x = camera_x + i * (lookx)*1.0;	
	camera_z = camera_z + i * (lookz)*1.0;
}

void orientMe(float ang) {
	lookx = sin(ang); 
	lookz = -cos(ang);
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
		jet_rotate += 5;
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.1f;
		orientMe(angle);
		playerLook -= 5;
		jet_rotate -= 5;
		break;
	}

	glutPostRedisplay();

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
