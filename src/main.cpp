/**
 * SER 431
 * https://speakerdeck.com/javiergs/ser431-lecture-04
 By Preston Goulet(pegoulet) & Taylor Greeff(tgreeff)

 TODO: fix camera; allow use of mouse to move
 **/

#include "Common.h"

vector<Points> box_spawn;

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

// display
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	float time = calculate_frame_time();
	updateParticles(time);
	updateGameObjects(time);
	updateLights();	

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
	gluLookAt(jet.position[0], jet.position[1], jet.position[2],
		jet.position[0] + jet.direction[1], jet.position[1] + jet.direction[0], jet.position[2] -  jet.direction[2],
		0.0f, 1.0f, 0.0f);

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

	//========================================
	//	Shadows
	//========================================
	glPushMatrix();
	// Tell GL new light source position
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	// Shadows
	if (areShadowsOn) {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	}
	// Draw floor using blending to blend in reflection
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0, 1.0, 1.0, 0.5);
	glPushMatrix();
	glDisable(GL_LIGHTING);

	//runway
	for (int x = 0; x < 10; x++) {
		glPushMatrix();
		glTranslatef(runway_x, runway_y, -(1800 * x) + runway_z);
		glCallList(runway);
		glPopMatrix();
	}

	glEnable(GL_LIGHTING);
	glPopMatrix();
	glDisable(GL_BLEND);

	// Shadows
	if (areShadowsOn) {
		glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
		glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		//  To eliminate depth buffer artifacts, use glEnable(GL_POLYGON_OFFSET_FILL);
		// Render 50% black shadow color on top of whatever the floor appareance is
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_LIGHTING);  /* Force the 50% black. */
		glColor4f(0.0, 0.0, 0.0, 0.5);
		glPushMatrix();
		// Project the shadow
		glMultMatrixf((GLfloat *)shadow_matrix);
		// boxes
		glDisable(GL_DEPTH_TEST);

		glTranslatef(camera_x, 20, camera_z - 500);
		glRotatef(180 + jet.rotation[1], 0.0, 1.0, 0.0);
		glRotatef(jet.rotation[0], 1.0, 0.0, 0.0);
		glTranslatef(lookx, looky, lookz);
		glScalef(10, 10, 10);
		glCallList(jetMesh);

		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
	}

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

	//================================
	//	Jet Reflection
	//================================
	glPushMatrix();	
		glScalef(1.0, -1.0, 1.0);
		glTranslatef(camera_x, 14, camera_z - 500);
		glRotatef(180 + jet.rotation[1], 0.0, 1.0, 0.0);
		glRotatef(jet.rotation[0], 1.0, 0.0, 0.0);
		glTranslatef(lookx, looky, lookz);
		glScalef(10, 10, 10);
		glCallList(jetMesh); //display7
	glPopMatrix();

	//============================================
	//	Exhaust flames reflection
	//============================================
	
	glPushMatrix();
	glScalef(1.0, -1.0, 1.0);
	glTranslatef(camera_x + 10, 5, camera_z - 400);
	glRotatef(90, 1, 0, 0);
	glRotatef(jet.rotation[1], 0.0, 1.0, 0.0);
	glScalef(.1, .1, .1);
	ps.drawParticles();//flames
	glPopMatrix();

	// STENCIL-STEP 4. disable it
	glDisable(GL_STENCIL_TEST);

	//=====================================
	//	Regular 3d environment
	//=====================================
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

	//Exhaust flames
	ps.remove();
	glPushMatrix();
	glTranslatef(camera_x + 12, camera_y - 60, camera_z - 450);
	glRotatef(90, 1, 0, 0);
	glRotatef(jet.rotation[1], 0.0, 1.0, 0.0);
	glScalef(.1, .1, .1);
	ps.drawParticles();//flames
	glPopMatrix();
	
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHT0);

	// jet
	glPushMatrix();
		glTranslatef(jet.position[0], jet.position[1], jet.position[2]);
		glRotatef(-90 + jet.rotation[0], 1.0, 0.0, 0.0);
		glRotatef(180 + jet.rotation[1], 0.0, 1.0, 0.0);
		glRotatef(jet.rotation[2], 0.0, 0.0, 1.0);
		
		glScalef(10, 10, 10);
		glCallList(jetMesh);

		if (areBoundingBoxesOn) {
			glDisable(GL_LIGHTING);
			glColor3f(1.0, 1.0, 0.0);
			glCallList(boundingBox);
			glEnable(GL_LIGHTING);
		}
		
	glPopMatrix();

	//land plane
	glPushMatrix();
		glTranslatef(-perlinMeshSize / 2, 0, -perlinMeshSize / 2);
		glCallList(display1);
	glPopMatrix();

	//runway
	for (int x = 0; x < 10; x++) {
		glPushMatrix();
		glTranslatef(runway_x, runway_y, -(1800 * x) + runway_z);
		glCallList(runway);
		glPopMatrix();
	}
	
	if (isLightArrowOn) {
		drawLightArrow();
	}

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
