/**
 * SER 431
 * https://speakerdeck.com/javiergs/ser431-lecture-04
 By Preston Goulet(pegoulet) & Taylor Greeff(tgreeff)

 TODO: fix camera; allow use of mouse to move
 **/

#include "Common.h"

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
	gluLookAt(camera_x, camera_y, camera_z,
		camera_x + lookx, camera_y + looky, camera_z + lookz,
		0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glScalef(scale, scale, scale);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glPopMatrix();

	// Raises and lowers the boxes to reveal the flames
	if (raiseAndLower) {
		if (lowerBox == false && boxMovement < 200) {
			boxMovement += 1;
			if (boxMovement > 199) {
				lowerBox = true;
			}
		}
		else {
			boxMovement -= 1;
			if (boxMovement == 0) {
				raiseAndLower = false;
				lowerBox = false;
				doOnce = true;
				generateRandomNumber();
			}
		}
	}

	//====================
	//	Draw Objects
	//====================
 	//flag
	glPushMatrix();
	GLfloat mat_diffuse[] = { 1.0f, 0.5f, 0.31f, 1. };
	GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1. };
	GLfloat mat_ambient[] = { 1.0f, 0.5f, 0.31f, 1. };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0);
	glTranslatef(400, shadowHeight + 700, playArea);
	glScalef(100, 100, 100);
	draw_nurb();
	glPopMatrix();

 	//flag reflection
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0);
	glTranslatef(400, shadowHeight - 700, playArea);
	glScalef(100, -100, 100);
	draw_nurb3();
	glPopMatrix();

 	// boxes reflections
	glPushMatrix();
	glTranslatef(0, shadowHeight, playArea);
	glScalef(1.0, -10.0, 1.0);
	glCallList(display2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(200, shadowHeight, playArea);
	glScalef(1.0, -1.0, 1.0);
	glCallList(display3);
	glPopMatrix();
	glPushMatrix();
	doOnce = false;
	glTranslatef(-200, shadowHeight, playArea);
	glScalef(1.0, -1.0, 1.0);
	glCallList(display4);
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
	glColor4f(1.0, 1.0, 1.0, 0.3);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glTranslatef(-900, shadowHeight, -900 + playArea);
	glCallList(displayBrick);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glDisable(GL_BLEND);
	// Shadows
	if (areShadowsOn) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_LIGHTING);  /* Force the 50% black. */
		glColor4f(0.0, 0.0, 0.0, 0.5);
		glPushMatrix();
		// Project the shadow
		glMultMatrixf((GLfloat *)shadow_matrix);
		// boxes
		glDisable(GL_DEPTH_TEST);
		glScalef(0, 10, 0);
		glCallList(display2);
		glTranslatef(200, shadowHeight, playArea);
		glCallList(display3);
		glTranslatef(-400, shadowHeight, playArea);
		glCallList(display4);
		glTranslatef(100, shadowHeight + 100, playArea + 100);
		glScalef(100, 100, 100);
		draw_nurb3();
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();

		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
		// To eliminate depth buffer artifacts, use glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_STENCIL_TEST);
	}

	// box 1
	glPushMatrix();
	glTranslatef(0, shadowHeight, playArea);
	glScalef(1.0, 10, 1.0);
	glCallList(display2);
	glPopMatrix();
	// box 2
	glPushMatrix();
	glTranslatef(200, shadowHeight + boxMovement, playArea);
	glCallList(display3);
	glPopMatrix();
	// box 3
	glPushMatrix();
	glTranslatef(-200, shadowHeight + boxMovement, playArea);
	glCallList(display4);
	glPopMatrix();
	glPopMatrix();

	//========Regular 3d environment=====================================
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.7, 0.0, 0.0, 0.9);
	glColor4f(1.0, 1.0, 1.0, 0.9);


	// skybox
	glPushMatrix();
	glTranslatef(-skyBoxMeshSize / 2, -skyBoxMeshSize / 2, -skyBoxMeshSize / 2);
	glCallList(display5);
	glPopMatrix();

	//Water
	glPushMatrix();
	glTranslatef(-boundaryMeshSize / 2, -50, -boundaryMeshSize / 2);
	glCallList(display6);
	glPopMatrix();

	////runway
	//for (int x = 0; x < 10; x++) {
	//	glPushMatrix();
	//	glTranslatef(runway_x, runway_y, -(1800 * x) + runway_z);
	//	glCallList(runway);
	//	glPopMatrix();
	//}

	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHT0);

	//============================================
	//	Exhaust flames
	//============================================

	if (areParticlesOn) {
		//Particles with box 1
		for (int i = 0; i < 50; i++) {
			ps.add();
		}
	}


	ps.update(time);

	if (leftBox) {
		glPushMatrix();
		glTranslatef(-150, shadowHeight, playArea + 50);
		glScalef(.1, .1, .1);
		ps.drawParticles();//flames
		glPopMatrix();

		ps.remove();
	}
	else {
		glPushMatrix();
		glTranslatef(250, shadowHeight, playArea + 50);
		glScalef(.1, .1, .1);
		ps.drawParticles();//flames
		glPopMatrix();

		ps.remove();
	}

	if (areBoundingBoxesOn) {
		//bounding box
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 0.0);
		glTranslatef(camera_x, camera_y - 100, camera_z - 500);
		glRotatef(180 + jet.rotation[1], 0.0, 1.0, 0.0);
		glTranslatef(lookx, looky, lookz);
		glScalef(10, 10, 10);
		glCallList(boundingBox);

		glEnable(GL_LIGHTING);
		glPopMatrix();
	}

	//jet
	glPushMatrix();
	glRotatef(jet.rotation[0], 1.0, 0.0, 0.0);
	glRotatef(jet.rotation[1], 0.0, 1.0, 0.0);
	glRotatef(jet.rotation[2], 0.0, 0.0, 1.0);
	Vec3f point = jetFollower.setNextPoint();
	if (jetFollower.indexAtEnd()) {
		jetFollower = CurveFollower(&jetCurveEnd);
	}
	printf("%d %d %d \n", point.x, point.y, point.z);
	glScalef(100, 100, 100);

	glTranslatef(point.x, point.y +100, point.z + 50);

	glCallList(jetMesh); 
	glPopMatrix();

	//plane
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
	countDown = countDown - 1;
	string countDownString = to_string(countDown);
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
	renderBitmapString((window_width / 2) - 50, (window_height / 10) * 9, 0.0f, "Time Left: ");
	renderBitmapString((window_width / 2) + 40, (window_height / 10) * 9, 0.0f, countDownString.c_str());
	if (getInBox) {
		renderBitmapString((window_width / 2) - 150, ((window_height / 10) * 9) + 20, 0.0f, "Go to the area with the Flag to make a choice!");
	}
	else if (correctChoice) {
		glColor3f(0.0, 0.0, 1.0);
		renderBitmapString((window_width / 2) - 50, ((window_height / 10) * 9) + 20, 0.0f, "Correct!!!!");
	}
	else {
		glColor3f(1.0, 0.0, 0.0);
		renderBitmapString((window_width / 2) - 50, ((window_height / 10) * 9) + 20, 0.0f, "Wrong :(");
	}

	string scoreString = to_string(totalScore);
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(10.0, window_height / 8, 0.0f, "Score: ");
	renderBitmapString(70, window_height / 8, 0.0f, scoreString.c_str());

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
	glVertex2f(0, 0);//bottom left
	glVertex2f(window_width, 0);//bottom right
	glVertex2f(window_width, window_height); //top right
	glVertex2f(0, window_height);//top left
	glEnd();

	glDisable(GL_CULL_FACE);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glutSwapBuffers();
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
